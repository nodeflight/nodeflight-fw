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

struct interface_serial_config_s {
    uint32_t baudrate;
    uint16_t tx_buf_size;
    uint16_t rx_buf_size;
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

static inline int interface_serial_configure(
    interface_header_t *iface,
    const interface_serial_config_t *config)
{
    if (iface->peripheral->decl->type != PERIPHERAL_SERIAL) {
        return -1;
    }
    interface_serial_t *if_ser = (interface_serial_t *) iface;
    return if_ser->configure(if_ser, config);
}

static inline int interface_serial_tx_write(
    interface_header_t *iface,
    void *buf,
    int bytes)
{
    if (iface->peripheral->decl->type != PERIPHERAL_SERIAL) {
        return -1;
    }
    interface_serial_t *if_ser = (interface_serial_t *) iface;
    return if_ser->tx_write(if_ser, buf, bytes);
}

static inline void interface_serial_tx_wait_done(
    interface_header_t *iface)
{
    if (iface->peripheral->decl->type != PERIPHERAL_SERIAL) {
        return;
    }
    interface_serial_t *if_ser = (interface_serial_t *) iface;
    if_ser->tx_wait_done(if_ser);
}
