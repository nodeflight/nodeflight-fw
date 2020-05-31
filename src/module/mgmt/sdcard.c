/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2020  Max Sikström
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/module.h"
#include "core/interface.h"
#include "core/interface_types.h"
#include "core/config.h"
#include "core/disk_access.h"
#include "lib/strops.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define SDCARD_SPI_BAUD_RATE        10000000UL
#define SDCARD_TASK_PRIORITY        2
#define SDCARD_INIT_RETRIES         10

#define SDCARD_TIMEOUT_ITERATIONS   50000

#define SDCARD_REQUEST_QUEUE_LENGTH 4

#define DEBUG_COMMANDS              0
#define DEBUG_CALLS                 0
#define DEBUG_ERRORS                0

#if DEBUG_COMMANDS
#include "vendor/tinyprintf/tinyprintf.h"
#define D_COMMAND_PRINTF(...) tfp_printf("sdcard " __VA_ARGS__);
#else
#define D_COMMAND_PRINTF(...) do {} while(0)
#endif

#if DEBUG_CALLS
#include "vendor/tinyprintf/tinyprintf.h"
#define D_CALL_PRINTF(...) tfp_printf("sdcard " __VA_ARGS__);
#else
#define D_CALL_PRINTF(...) do {} while(0)
#endif

#if DEBUG_ERRORS
#include "vendor/tinyprintf/tinyprintf.h"
#define D_ERROR_PRINTF(...) tfp_printf("sdcard " __VA_ARGS__);
#else
#define D_ERROR_PRINTF(...) do {} while(0)
#endif

typedef struct sdcard_s sdcard_t;
typedef struct sdcard_job_s sdcard_job_t;

typedef enum sdcard_job_id_s {
    SDCARD_JOB_ID_INITIALIZE,
    SDCARD_JOB_ID_STATUS,
    SDCARD_JOB_ID_READ,
    SDCARD_JOB_ID_WRITE,
    SDCARD_JOB_ID_IOCTL
} sdcard_job_id_t;

struct sdcard_s {
    if_spi_t *if_spi;
    if_gpio_t *if_cs;

    TaskHandle_t task;
    QueueHandle_t jobs;

    bool card_available;
    uint32_t reg_ocr;
    uint8_t reg_csd[16];
    uint8_t reg_cid[16];
};

struct sdcard_job_s {
    TaskHandle_t calling_task;
    sdcard_job_id_t job_id;

    union {
        struct {
            BYTE *buff;
            LBA_t sector;
            UINT count;
        } read;
        struct {
            const BYTE *buff;
            LBA_t sector;
            UINT count;
        } write;
        struct {
            BYTE cmd;
            void *buff;
        } ioctl;
    } arg;
};

static int sdcard_init(
    const char *name,
    md_arg_t *args);

static uint8_t sdcard_crc7(
    uint8_t *ptr,
    int len);

static uint16_t sdcard_crc16(
    uint8_t *ptr,
    int len);

static uint8_t sdcard_wait_token(
    sdcard_t *sdc);

static int sdcard_send_command(
    sdcard_t *sdc,
    uint8_t command,
    uint32_t arg);

static uint8_t sdcard_read_r1(
    sdcard_t *sdc);

static uint8_t sdcard_read_r3(
    sdcard_t *sdc,
    uint32_t *var);

static int sdcard_read_block(
    sdcard_t *sdc,
    uint8_t command,
    uint32_t arg,
    uint8_t data_token,
    uint8_t *dst,
    int size);

static int sdcard_init_card(
    sdcard_t *sdc);

static void sdcard_disconnect(
    sdcard_t *sdc);

static void sdcard_task(
    void *storage);

static DSTATUS sdcard_dacc_initialize(
    void *storage);

static DSTATUS sdcard_dacc_status(
    void *storage);

static DRESULT sdcard_dacc_read(
    void *storage,
    BYTE *buff,
    LBA_t sector,
    UINT count);

static DRESULT sdcard_dacc_write (
    void *storage,
    const BYTE *buff,
    LBA_t sector,
    UINT count);

static DRESULT sdcard_dacc_ioctl (
    void *storage,
    BYTE cmd,
    void *buff);

MD_DECL(sdcard, "pp", sdcard_init);

int sdcard_init(
    const char *name,
    md_arg_t *args)
{
    if (args[0].iface->peripheral->decl->type != PP_SPI) {
        return -1;
    }
    if (args[1].iface->peripheral->decl->type != PP_GPIO) {
        return -1;
    }
    if(name == NULL) {
        /* SD card needs a name/mount point */
        return -1;
    }

    sdcard_t *sdc;
    disk_access_t *dacc;

    sdc = pvPortMalloc(sizeof(sdcard_t));
    if (sdc == NULL) {
        return -1;
    }

    sdc->if_spi = IF_SPI(args[0].iface);
    sdc->if_spi->configure(sdc->if_spi, &(if_spi_cf_t) {
        .max_baud_rate = SDCARD_SPI_BAUD_RATE,
        .mode = SPI_MODE_LEADING_LOW
    });

    sdc->if_cs = IF_GPIO(args[1].iface);
    sdc->if_cs->configure(sdc->if_cs, &(if_gpio_cf_t) {
        .dir = GPIO_DIR_OUT,
        .pull = GPIO_PULL_NONE
    });
    sdc->if_cs->set_value(sdc->if_cs, true);

    sdc->card_available = false;

    sdc->jobs = xQueueCreate(SDCARD_REQUEST_QUEUE_LENGTH, sizeof(sdcard_job_t));
    if (sdc->jobs == NULL) {
        return -1;
    }

    dacc = pvPortMalloc(sizeof(disk_access_t));
    if (dacc == NULL) {
        return -1;
    }
    dacc->name = strops_dup(name);
    dacc->initialize = sdcard_dacc_initialize;
    dacc->status = sdcard_dacc_status;
    dacc->read = sdcard_dacc_read;
    dacc->write = sdcard_dacc_write;
    dacc->ioctl = sdcard_dacc_ioctl;
    dacc->storage = sdc;

    if (0 != disk_access_register(dacc)) {
        return -1;
    }

    xTaskCreate(sdcard_task, "sdcard", 1024, sdc, SDCARD_TASK_PRIORITY, &sdc->task);
    return 0;
}

uint8_t sdcard_crc7(
    uint8_t *ptr,
    int len)
{
    uint16_t crc = 0;
    int i;
    int bit;
    for (i = 0; i < len; i++) {
        crc ^= ptr[i];
        for (bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {
                crc ^= 0x89;
            }
            crc <<= 1;
        }
    }
    /* crc7 seems to always be stored in msb 7bits with 1 at lsb */
    return crc | 1;
}

uint16_t sdcard_crc16(
    uint8_t *ptr,
    int len)
{
    uint16_t crc = 0;
    int i;
    int bit;
    for (i = 0; i < len; i++) {
        crc ^= ((uint16_t) ptr[i] << 8);
        for (bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc <<= 1;
                crc ^= 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint8_t sdcard_wait_token(
    sdcard_t *sdc)
{
    uint8_t rx;
    int time_left = SDCARD_TIMEOUT_ITERATIONS;
    do {
        sdc->if_spi->transfer(sdc->if_spi, NULL, &rx, 1);
    } while(rx == 0xff && time_left--);
    return rx;
}

int sdcard_send_command(
    sdcard_t *sdc,
    uint8_t command,
    uint32_t arg)
{
    uint8_t buf[6];

    int time_left = SDCARD_TIMEOUT_ITERATIONS;

    /* Wait until not busy */
    do {
        sdc->if_spi->transfer(sdc->if_spi, NULL, buf, 1);
        if (time_left-- == 0) {
            return -1;
        }
    } while(buf[0] != 0xff);

    /* Pack command and send */
    buf[0] = 0x40 | command;
    buf[1] = (arg >> 24) & 0xff;
    buf[2] = (arg >> 16) & 0xff;
    buf[3] = (arg >> 8) & 0xff;
    buf[4] = (arg >> 0) & 0xff;

    buf[5] = sdcard_crc7(buf, 5); /* CRC + end bit */

    sdc->if_spi->transfer(sdc->if_spi, buf, NULL, 6);

    D_COMMAND_PRINTF("cmd=%3u: %02x | %02x %02x %02x %02x | %02x\n",
        command,
        buf[0],
        buf[1],
        buf[2],
        buf[3],
        buf[4],
        buf[5]);

    return 0;
}

uint8_t sdcard_read_r1(
    sdcard_t *sdc)
{
    uint8_t r1;
    int time_left = SDCARD_TIMEOUT_ITERATIONS;
    do {
        sdc->if_spi->transfer(sdc->if_spi, NULL, &r1, 1);
    } while((r1 & 0x80) != 0 && time_left--);
    return r1;
}

uint8_t sdcard_read_r3(
    sdcard_t *sdc,
    uint32_t *var)
{
    uint8_t r1 = sdcard_read_r1(sdc);
    uint8_t buf[4];
    if ((r1 & 0x80) == 0) {
        sdc->if_spi->transfer(sdc->if_spi, NULL, buf, 4);
        *var = ((uint32_t) buf[0]) << 24
            | ((uint32_t) buf[1]) << 16
            | ((uint32_t) buf[2]) << 8
            | ((uint32_t) buf[3]) << 0;
    } else {
        *var = 0;
    }
    return r1;
}

int sdcard_read_block(
    sdcard_t *sdc,
    uint8_t command,
    uint32_t arg,
    uint8_t data_token,
    uint8_t *dst,
    int size)
{
    uint8_t crc[2];
    uint8_t r1;
    uint16_t exp_crc;

    sdc->if_cs->set_value(sdc->if_cs, false);

    /* CMD=9 (SEND_CSD) */
    if (0 != sdcard_send_command(sdc, command, arg)) {
        goto error;
    }
    r1 = sdcard_read_r1(sdc);
    if (r1 != 0x00) {
        goto error;
    }

    if (sdcard_wait_token(sdc) != data_token) {
        goto error;
    }
    sdc->if_spi->transfer(sdc->if_spi, NULL, dst, size);
    sdc->if_spi->transfer(sdc->if_spi, NULL, crc, 2);

    exp_crc = sdcard_crc16(dst, size);
    if (crc[0] != (exp_crc >> 8) || crc[1] != (exp_crc & 0xff)) {
        goto error;
    }

    sdc->if_cs->set_value(sdc->if_cs, true);
    return 0;
error:
    sdc->if_cs->set_value(sdc->if_cs, true);
    return -1;
}

int sdcard_init_card(
    sdcard_t *sdc)
{
    uint8_t r1;
    uint32_t var;

    /* Startup with >=74 clock pulses deselected */
    sdc->if_cs->set_value(sdc->if_cs, true);
    sdc->if_spi->transfer(sdc->if_spi, NULL, NULL, 16);
    sdc->if_cs->set_value(sdc->if_cs, false);

    /* Send CMD0 (GO_IDLE_STATE) - reset the SD card */
    if (0 != sdcard_send_command(sdc, 0, 0)) {
        goto error;
    }
    r1 = sdcard_read_r1(sdc);
    if (r1 != 0x01) {
        goto error;
    }

    /* Send CMD=8 - illegal for SDC 1 and MMC */
    if (0 != sdcard_send_command(sdc, 8, 0x000001aa)) {
        goto error;
    }
    r1 = sdcard_read_r3(sdc, &var);
    if (r1 != 0x01 || (var & 0x000001ff) != 0x000001aa) {
        /* Only SDHC supported */
        goto error;
    }

    /* ACMD=42 - Synchronize and negotiate operation voltage */
    do {
        /* Send CMD=5 (APP_CMD) - Next command is app command */
        if (0 != sdcard_send_command(sdc, 55, 0)) {
            goto error;
        }
        r1 = sdcard_read_r1(sdc);
        if (r1 != 0x01) {
            goto error;
        }

        /* Send ACMD=41 (APP_CMD) - Next command is app command */
        if (0 != sdcard_send_command(sdc, 41, 0x40000000)) {
            goto error;
        }
        r1 = sdcard_read_r1(sdc);
        if (r1 != 0x01 && r1 != 0x00) {
            goto error;
        }
    } while(r1 != 0x00);

    /* Finish by reading OCR register */

    /* CMD=58 (READ_OCR) */
    if (0 != sdcard_send_command(sdc, 58, 0)) {
        goto error;
    }
    r1 = sdcard_read_r3(sdc, &sdc->reg_ocr);
    if (r1 != 0x00) {
        goto error;
    }

    sdc->if_cs->set_value(sdc->if_cs, true);

    /* Card operation complete, read card information */

    /* Read CSD */
    if (sdcard_read_block(sdc, 9, 0, 0xfe, sdc->reg_csd, 16) != 0) {
        D_ERROR_PRINTF("sdcard:! can't read CSD\n");
        goto error;
    }

    /* Only allow high capacity cards (SDHC/SDXC), for now */
    if ((sdc->reg_ocr & 0x40000000) == 0) {
        D_ERROR_PRINTF("sdcard:! OCR missing high capacity bit\n");
        goto error;
    }

    /* Only allow CSD version 2, which is SDHC/SDXC cards */
    if ((sdc->reg_csd[0] & 0xc0) != 0x40) {
        D_ERROR_PRINTF("sdcard:! invalid CSD version %u\n", sdc->reg_csd[0] >> 6);
        goto error;
    }

    /* Read CID */
    if (sdcard_read_block(sdc, 10, 0, 0xfe, sdc->reg_cid, 16) != 0) {
        D_ERROR_PRINTF("sdcard:! can't read CID\n");
        goto error;
    }

    /* Verify CID CRC */
    if (sdcard_crc7(sdc->reg_cid, 15) != sdc->reg_cid[15]) {
        D_ERROR_PRINTF("sdcard:! invalid CID CRC\n");
        goto error;
    }

    sdc->card_available = true;
    return 0;

error:
    sdc->if_cs->set_value(sdc->if_cs, true);
    return -1;
}

void sdcard_disconnect(
    sdcard_t *sdc)
{
    int i;
    sdc->card_available = false;

    /* Clear card specific info */
    sdc->reg_ocr = 0;
    for (i = 0; i < 16; i++) {
        sdc->reg_csd[i] = 0;
    }
    for (i = 0; i < 16; i++) {
        sdc->reg_cid[i] = 0;
    }
}

void sdcard_task(
    void *storage)
{
    sdcard_t *sdc = (sdcard_t *) storage;
    sdcard_job_t job;
    uint8_t result;

    for (;;) {
        if (pdFALSE != xQueueReceive(sdc->jobs, &job, portMAX_DELAY)) {
            result = 0;
            switch (job.job_id) {
            case SDCARD_JOB_ID_INITIALIZE:
            {
                int retries = SDCARD_INIT_RETRIES;
                do {
                    if (sdcard_init_card(sdc) < 0) {
                        /* Error initialization */
                        result = 1;
                    } else {
                        result = 0;
                    }
                } while(result != 0 && --retries);
            }
            break;

            case SDCARD_JOB_ID_STATUS:
            {
                result = sdc->card_available ? 0 : STA_NODISK;
            }
            break;

            case SDCARD_JOB_ID_READ:
            {
                int i;
                result = RES_OK;
                for (i = 0; i < job.arg.read.count && result == RES_OK; i++) {
                    if (0 != sdcard_read_block(
                        sdc,
                        17,
                        job.arg.read.sector + i,
                        0xfe,
                        job.arg.read.buff + 512 * i,
                        512)
                    ) {
                        result = RES_ERROR;
                    }
                }
            }
            break;

            case SDCARD_JOB_ID_WRITE:
            {
                /* Not yet implemented */
                result = RES_ERROR;
            }

            case SDCARD_JOB_ID_IOCTL:
            {
                /* Not yet implemented */
                result = RES_ERROR;
            }
            break;
            }
            xTaskNotify(job.calling_task, 0x01000000 | (((uint32_t) result) << 25), eSetBits);
        }
    }

    (void) sdcard_disconnect;
}

uint8_t sdcard_dacc_exec_job(
    sdcard_t *sdc,
    sdcard_job_t *job)
{
    uint32_t notify_value;
    job->calling_task = xTaskGetCurrentTaskHandle();

    ulTaskNotifyValueClear(NULL, 0xff000000);
    if (pdFALSE != xQueueSendToBack(sdc->jobs, job, portMAX_DELAY)) {
        xTaskNotifyWait(0, 0xff000000, &notify_value, portMAX_DELAY);
        uint8_t result = notify_value >> 25;
        return result;
    } else {
        return 0xff;
    }
}

DSTATUS sdcard_dacc_initialize(
    void *storage)
{
    D_CALL_PRINTF("sdcard_dacc_initialize()\n");
    uint8_t result = sdcard_dacc_exec_job((sdcard_t *) storage, &(sdcard_job_t) {
        .job_id = SDCARD_JOB_ID_INITIALIZE
    });
    if (result == 0) {
        return 0;
    } else {
        return STA_NODISK;
    }
}

DSTATUS sdcard_dacc_status(
    void *storage)
{
    D_CALL_PRINTF("sdcard_dacc_status()\n");
    uint8_t result = sdcard_dacc_exec_job((sdcard_t *) storage, &(sdcard_job_t) {
        .job_id = SDCARD_JOB_ID_STATUS
    });
    if (result != 0xff) {
        return result;
    } else {
        return STA_NODISK;
    }
}

DRESULT sdcard_dacc_read(
    void *storage,
    BYTE *buff,
    LBA_t sector,
    UINT count)
{
    D_CALL_PRINTF("sdcard_dacc_read(%p, %lu, %u)\n", buff, sector, count);
    uint8_t result = sdcard_dacc_exec_job((sdcard_t *) storage, &(sdcard_job_t) {
        .job_id = SDCARD_JOB_ID_READ,
        .arg.read = {
            .buff = buff,
            .sector = sector,
            .count = count
        }
    });
    if (result == 0xff) {
        return RES_ERROR;
    } else {
        return (DRESULT) result;
    }
}

DRESULT sdcard_dacc_write (
    void *storage,
    const BYTE *buff,
    LBA_t sector,
    UINT count)
{
    D_CALL_PRINTF("sdcard_dacc_write(%p, %lu, %u)\n", buff, sector, count);
    uint8_t result = sdcard_dacc_exec_job((sdcard_t *) storage, &(sdcard_job_t) {
        .job_id = SDCARD_JOB_ID_WRITE,
        .arg.write = {
            .buff = buff,
            .sector = sector,
            .count = count
        }
    });
    if (result == 0xff) {
        return RES_ERROR;
    } else {
        return (DRESULT) result;
    }
}

DRESULT sdcard_dacc_ioctl (
    void *storage,
    BYTE cmd,
    void *buff)
{
    D_CALL_PRINTF("sdcard_dacc_ioctl(%u, %p)\n", cmd, buff);
    uint8_t result = sdcard_dacc_exec_job((sdcard_t *) storage, &(sdcard_job_t) {
        .job_id = SDCARD_JOB_ID_WRITE,
        .arg.ioctl = {
            .cmd = cmd,
            .buff = buff
        }
    });
    if (result == 0xff) {
        return RES_ERROR;
    } else {
        return (DRESULT) result;
    }
}
