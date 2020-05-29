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

#define SDCARD_SPI_BAUD_RATE        2000000UL
#define SDCARD_TASK_PRIORITY        2

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

void sdcard_task(
    void *storage)
{
    sdcard_t *sdc = (sdcard_t *) storage;

    (void)sdc;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(250));

    }
}
