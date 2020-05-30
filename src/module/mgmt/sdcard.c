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
#include "FreeRTOS.h"
#include "task.h"

#include "vendor/tinyprintf/tinyprintf.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_PACKET_LENGTH           256

#define SDCARD_SPI_BAUD_RATE        10000000UL
#define SDCARD_TASK_PRIORITY        2

#define SDCARD_TIMEOUT_ITERATIONS   50000

#define DEBUG_COMMANDS              0

typedef struct sdcard_s sdcard_t;

struct sdcard_s {
    if_spi_t *if_spi;
    if_gpio_t *if_cs;

    TaskHandle_t task;

    bool card_available;
    uint32_t reg_ocr;
    uint8_t reg_csd[16];
};

static int sdcard_init(
    const char *name,
    md_arg_t *args);

static void sdcard_task(
    void *storage);

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

    sdcard_t *sdc;

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

    xTaskCreate(sdcard_task, "sdcard", 1024, sdc, SDCARD_TASK_PRIORITY, &sdc->task);
    return 0;
}

static uint8_t sdcard_crc7(
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
    /* Compensate for loop shifting after xor instead of xor if shift out */
    return crc >> 1;
}

static uint16_t sdcard_crc16(
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

static uint8_t sdcard_wait_token(
    sdcard_t *sdc)
{
    uint8_t rx;
    int time_left = SDCARD_TIMEOUT_ITERATIONS;
    do {
        sdc->if_spi->transfer(sdc->if_spi, NULL, &rx, 1);
    } while(rx == 0xff && time_left--);
    return rx;
}

static int sdcard_send_command(
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

    buf[5] = (sdcard_crc7(buf, 5) << 1) | 1; /* CRC + end bit */

    sdc->if_spi->transfer(sdc->if_spi, buf, NULL, 6);

#if DEBUG_COMMANDS
    tfp_printf("sdcard: cmd=%3u: %02x | %02x %02x %02x %02x | %02x\n",
        command,
        buf[0],
        buf[1],
        buf[2],
        buf[3],
        buf[4],
        buf[5]);
#endif

    return 0;
}

static uint8_t sdcard_read_r1(
    sdcard_t *sdc)
{
    uint8_t r1;
    int time_left = SDCARD_TIMEOUT_ITERATIONS;
    do {
        sdc->if_spi->transfer(sdc->if_spi, NULL, &r1, 1);
    } while((r1 & 0x80) != 0 && time_left--);
    return r1;
}

static uint8_t sdcard_read_r3(
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

    sdc->card_available = true;

    sdc->if_cs->set_value(sdc->if_cs, true);
    return 0;

error:
    sdc->if_cs->set_value(sdc->if_cs, true);
    return -1;
}

static int sdcard_read_card_info(
    sdcard_t *sdc)
{
    uint8_t crc[2];
    uint8_t r1;
    uint16_t exp_crc;
    /* Can't read card info if card isn't conneted */
    if (!sdc->card_available) {
        return -1;
    }

    /* Only allow high capacity cards (SDHC/SDXC), for now */
    if ((sdc->reg_ocr & 0x40000000) == 0) {
        return -1;
    }

    sdc->if_cs->set_value(sdc->if_cs, false);

    /* CMD=9 (SEND_CSD) */
    if (0 != sdcard_send_command(sdc, 9, 0)) {
        goto error;
    }
    r1 = sdcard_read_r1(sdc);
    if (r1 != 0x00) {
        goto error;
    }
    /* DATA_TOKEN_CMD9 = 0xfe */
    if (sdcard_wait_token(sdc) != 0xfe) {
        goto error;
    }
    sdc->if_spi->transfer(sdc->if_spi, NULL, sdc->reg_csd, 16);
    sdc->if_spi->transfer(sdc->if_spi, NULL, crc, 2);

    exp_crc = sdcard_crc16(sdc->reg_csd, 16);
    if (crc[0] != (exp_crc >> 8) || crc[1] != (exp_crc & 0xff)) {
        goto error;
    }

    /* Only allow CSD version 2, which is SDHC/SDXC cards */
    if ((sdc->reg_csd[0] & 0xc0) != 0x40) {
        goto error;
    }

    sdc->if_cs->set_value(sdc->if_cs, true);
    return 0;
error:
    sdc->if_cs->set_value(sdc->if_cs, true);
    return -1;
}

static void sdcard_disconnect(
    sdcard_t *sdc)
{
    int i;
    sdc->card_available = false;

    /* Clear card specific info */
    sdc->reg_ocr = 0;
    for (i = 0; i < 16; i++) {
        sdc->reg_csd[i] = 0;
    }
}

void sdcard_task(
    void *storage)
{
    sdcard_t *sdc = (sdcard_t *) storage;
    int status;

    /* Just to make sure to start after system start, and power rails are loaded */
    vTaskDelay(pdMS_TO_TICKS(250));

    for (;;) {
        do {
            tfp_printf("sdcard: starting\n");
            status = sdcard_init_card(sdc);
        } while(status != 0);
        tfp_printf("sdcard: started\n");

        if (sdcard_read_card_info(sdc) != 0) {
            sdcard_disconnect(sdc);
        }

        tfp_printf("csd  0: %02x %02x %02x %02x\n",
            sdc->reg_csd[0],
            sdc->reg_csd[1],
            sdc->reg_csd[2],
            sdc->reg_csd[3]);
        tfp_printf("csd  4: %02x %02x %02x %02x\n",
            sdc->reg_csd[4],
            sdc->reg_csd[5],
            sdc->reg_csd[6],
            sdc->reg_csd[7]);
        tfp_printf("csd  8: %02x %02x %02x %02x\n",
            sdc->reg_csd[8],
            sdc->reg_csd[9],
            sdc->reg_csd[10],
            sdc->reg_csd[11]);
        tfp_printf("csd 12: %02x %02x %02x %02x\n",
            sdc->reg_csd[12],
            sdc->reg_csd[13],
            sdc->reg_csd[14],
            sdc->reg_csd[15]);

        while (sdc->card_available) {
            vTaskDelay(pdMS_TO_TICKS(250));
        }
        tfp_printf("sdcard: disconnect\n");
    }
}
