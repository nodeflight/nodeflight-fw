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

typedef struct usb_vcp_if_s usb_vcp_if_t;

struct usb_vcp_if_s {
    uint8_t dummy;
};

static int usb_vcp_init(
    if_header_t *iface,
    const char *config);

PP_TYPE_DECL(usb_vcp, PP_SERIAL, 0, usb_vcp_init, sizeof(usb_vcp_if_t));

static int usb_vcp_init(
    if_header_t *iface,
    const char *config)
{
    /* TODO: Implement... */
    return -1;
}
