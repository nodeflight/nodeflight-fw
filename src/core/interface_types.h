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

#pragma once

#include "core/interface.h"

typedef struct interface_serial_s interface_serial_t;
typedef struct interface_serial_config_s interface_serial_config_t;

#define INTERFACE_SERIAL_INVERTED_TX 0x00000001
#define INTERFACE_SERIAL_INVERTED_RX 0x00000002
#define INTERFACE_SERIAL_HALF_DUPLEX 0x00000004

struct interface_serial_config_s {
    uint32_t baudrate;
    uint16_t tx_buf_size;
    uint16_t rx_buf_size;
    uint32_t flags;

    void (*rx_done)(
        const uint8_t *buf,
        uint16_t len,
        void *storage);

    void *storage;
};

struct interface_serial_s {
    interface_header_t header;

    int (*configure)(
        interface_serial_t *iface,
        const interface_serial_config_t *config);

    int (*tx_write)(
        interface_serial_t *iface,
        const void *buf,
        int bytes);

    void (*tx_wait_done)(
        interface_serial_t *iface);
};

#define INTERFACE_SERIAL(_INTERFACE) ((interface_serial_t *) (_INTERFACE))
