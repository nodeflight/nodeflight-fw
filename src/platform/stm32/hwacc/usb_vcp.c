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

#include "core/peripheral.h"

typedef struct usb_vcp_interface_s usb_vcp_interface_t;

struct usb_vcp_interface_s {
    uint8_t dummy;
};

static int usb_vcp_init(
    interface_header_t *iface,
    const char *config);

PERIPHERAL_TYPE_DECL(usb_vcp, PERIPHERAL_SERIAL, 0, usb_vcp_init, sizeof(usb_vcp_interface_t));

static int usb_vcp_init(
    interface_header_t *iface,
    const char *config)
{
    /* TODO: Implement... */
    return -1;
}
