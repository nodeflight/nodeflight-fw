/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2023  Max Sikström
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

#include "core/peripheral.h"
#include "core/interface.h"
#include "core/interface_types.h"

typedef struct usb_vcp_if_s usb_vcp_if_t;

struct usb_vcp_if_s {
    if_serial_t header;
};

static int usb_vcp_init(
    if_header_t *iface);

static int usb_vcp_configure(
    if_serial_t *iface,
    const if_serial_cf_t *config);

static int usb_vcp_tx_write(
    if_serial_t *iface,
    const void *buf,
    int bytes);

static void usb_vcp_tx_wait_done(
    if_serial_t *iface,
    TickType_t timeout);

PP_TYPE_DECL(usb_vcp, PP_SERIAL, 0, usb_vcp_init, sizeof(usb_vcp_if_t));

int usb_vcp_init(
    if_header_t *iface)
{
    usb_vcp_if_t *vcp = (usb_vcp_if_t *) iface;

    vcp->header.configure = usb_vcp_configure;
    vcp->header.tx_write = usb_vcp_tx_write;
    vcp->header.tx_wait_done = usb_vcp_tx_wait_done;

    return 0;
}

int usb_vcp_configure(
    if_serial_t *iface,
    const if_serial_cf_t *config)
{
    return 0;
}

int usb_vcp_tx_write(
    if_serial_t *iface,
    const void *buf,
    int bytes)
{
    return 0;
}

void usb_vcp_tx_wait_done(
    if_serial_t *iface,
    TickType_t timeout)
{

}
