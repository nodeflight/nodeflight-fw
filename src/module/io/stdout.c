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

/**
 * Stdout - handle status output over serial port
 *
 * This module is a singleton, which means only one may exist at a time.
 *
 * It is intended for status from other modules, where status isn't required for real time handling.
 *
 * To send information out over this port, use the tfp_printf() function available in vendor/tinyprintf/tinyprintf.h
 *
 * Output is linebuffered, and will be transmitted in bursts of lines, if lines are short.
 *
 * Writing to the output is blocking in the case of full buffer.
 */

#include "core/module.h"
#include "core/interface.h"
#include "core/interface_types.h"
#include "core/config.h"
#include "FreeRTOS.h"

#include "vendor/tinyprintf/tinyprintf.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 64

typedef struct stdout_s stdout_t;

struct stdout_s {
    if_header_t *if_stdout;
    char *buffer;
    uint16_t pos;
    uint16_t size;
};

static int stdout_init(
    const char *tag);

MODULE_DECL(stdout, stdout_init);

/**
 * Guard for multiple allocations
 */
static bool stdout_loaded = false;

static void stdout_putc(
    void *storage,
    char c)
{
    stdout_t *out_if = storage;
    out_if->buffer[out_if->pos++] = c;
    if (out_if->pos >= out_if->size || c == '\n') {
        IF_SERIAL(out_if->if_stdout)->tx_write(IF_SERIAL(out_if->if_stdout), out_if->buffer, out_if->pos);
        out_if->pos = 0;
    }
}

int stdout_init(
    const char *tag)
{
    const char *pp_config;
    stdout_t *out_if;

    if (stdout_loaded) {
        /* Only one stdout module may be used at a time */
        return -1;
    }
    pp_config = config_get_pp_config(tag);
    if (pp_config == NULL) {
        return -1;
    }

    out_if = pvPortMalloc(sizeof(stdout_t));
    if (out_if == NULL) {
        return -1;
    }

    out_if->size = MAX_LINE_LENGTH;
    out_if->pos = 0;
    out_if->buffer = pvPortMalloc(MAX_LINE_LENGTH);
    if (out_if->buffer == NULL) {
        return -1;
    }
    out_if->if_stdout = if_create(pp_config, PP_SERIAL);
    if (out_if->if_stdout == NULL) {
        return -1;
    }
    IF_SERIAL(out_if->if_stdout)->configure(IF_SERIAL(out_if->if_stdout),
        &(const if_serial_config_t) {
        .baudrate = 115200,
        .tx_buf_size = MAX_LINE_LENGTH,
        .rx_buf_size = 0,
        .flags = 0
    });

    init_printf(out_if, stdout_putc);

    stdout_loaded = true;

    return 0;
}
