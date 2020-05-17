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
#include <string.h>

typedef struct fport_s fport_t;

struct fport_s {
    interface_header_t *if_ser;

    uint8_t data[64];
    uint16_t len;

    TaskHandle_t task;
};

static int fport_init(
    const char *tag);

static void fport_task(
    void *storage);

MODULE_DECL(fport, fport_init);

static void fport_rx_done(
    const uint8_t *buf,
    uint16_t len,
    void *storage)
{
    fport_t *fport_if = storage;

    memcpy(fport_if->data, buf, len);
    fport_if->len = len;

    BaseType_t should_switch = pdFALSE;
    xTaskNotifyFromISR(fport_if->task, 0x00000001, eSetBits, &should_switch);
    portYIELD_FROM_ISR(should_switch);
}

int fport_init(
    const char *tag)
{
    const char *peripheral_config;
    fport_t *fport_if;

    peripheral_config = config_get_peripheral_config(tag);
    if (peripheral_config == NULL) {
        return -1;
    }

    fport_if = pvPortMalloc(sizeof(fport_t));
    if (fport_if == NULL) {
        return -1;
    }

    xTaskCreate(fport_task,
        "fport_proc",
        1024,
        fport_if,
        15,
        &fport_if->task);

    fport_if->if_ser = interface_create(peripheral_config);
    if (fport_if->if_ser == NULL) {
        return -1;
    }
    interface_serial_configure(fport_if->if_ser, &(const interface_serial_config_t) {
        .baudrate = 115200,
        .tx_buf_size = 16,
        .rx_buf_size = 128,
        .flags = (
            INTERFACE_SERIAL_HALF_DUPLEX
            | INTERFACE_SERIAL_INVERTED_RX
            | INTERFACE_SERIAL_INVERTED_TX
            ),
        .rx_done = fport_rx_done,
        .storage = fport_if
    });

    return 0;
}

void fport_task(
    void *storage)
{
    fport_t *fport_if = storage;
    int i;
    uint32_t notify_value;

    tfp_printf("fport loaded\n");

    for (;;) {
        xTaskNotifyWait(0x00, UINT32_MAX, &notify_value, 500 * portTICK_PERIOD_MS);
        tfp_printf("fport:");
        for (i = 0; i < fport_if->len; i++) {
            tfp_printf(" %02x", fport_if->data[i]);
        }
        tfp_printf("\n");
    }
}
