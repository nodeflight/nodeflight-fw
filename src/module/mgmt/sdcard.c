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

typedef struct sdcard_s sdcard_t;

struct sdcard_s {
    if_spi_t *if_spi;
    if_gpio_t *if_cs;

    TaskHandle_t task;
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

    xTaskCreate(sdcard_task, "sdcard", 1024, sdc, SDCARD_TASK_PRIORITY, &sdc->task);
    return 0;
}

static int sdcard_send_command(
    sdcard_t *sdc,
    uint8_t command,
    uint32_t arg)
{
    uint8_t buf[6];
    uint8_t crc;
    int i;
    int bit;

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

    crc = 0;
    for (i = 0; i < 5; i++) {
        crc ^= buf[i];
        for (bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {
                crc ^= 0x89;
            }
            crc <<= 1;
        }
    }
    /* crc is already shifted */
    buf[5] = crc | 1; /* CRC + end bit */

    sdc->if_spi->transfer(sdc->if_spi, buf, NULL, 6);

    tfp_printf("sdcard: cmd=%3u: %02x | %02x %02x %02x %02x | %02x\n",
        command,
        buf[0],
        buf[1],
        buf[2],
        buf[3],
        buf[4],
        buf[5]);
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
    r1 = sdcard_read_r3(sdc, &var);
    if (r1 != 0x00) {
        goto error;
    }
    tfp_printf("OCR: %08lx\n", var);

    sdc->if_cs->set_value(sdc->if_cs, true);
    return 0;

error:
    sdc->if_cs->set_value(sdc->if_cs, true);
    return -1;
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

        for (;;) {
            vTaskDelay(pdMS_TO_TICKS(250));
        }
    }
}
