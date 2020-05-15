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

#include "vendor/tinyprintf/tinyprintf.h"

#include <stdint.h>
#include <stddef.h>

interface_header_t *if_stdout;

static int stdout_init(
    const char *tag);

MODULE_DECL(stdout, stdout_init);

static void stdout_putc(
    void *storage,
    char c)
{
    interface_header_t *if_stdout = storage;
    interface_serial_tx_write(if_stdout, &c, 1);
}

int stdout_init(
    const char *tag)
{
    interface_header_t *if_stdout;
    const char *peripheral_config;

    peripheral_config = config_get_peripheral_config(tag);
    if_stdout = interface_create(peripheral_config);
    if (if_stdout == NULL) {
        return -1;
    }

    init_printf(if_stdout, stdout_putc);

    return 0;
}
