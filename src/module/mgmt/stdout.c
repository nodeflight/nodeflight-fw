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
 * It is intended for status from other modules, where status isn't required for real time handling.
 *
 * To send information out over this port, use the log_printf() function available in core/log.
 */

#include "core/module.h"
#include "core/interface.h"
#include "core/interface_types.h"
#include "core/config.h"
#include "FreeRTOS.h"

#include "core/log.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "vendor/tinyprintf/tinyprintf.h"

#define STDOUT_LINE_LENGTH 128

typedef struct stdout_s stdout_t;

struct stdout_s {
    if_serial_t *iface;
    char buffer[STDOUT_LINE_LENGTH];
};

static int stdout_init(
    const md_decl_t *md,
    const char *name,
    md_arg_t *args);

static void stdout_log_handler(
    TaskHandle_t task,
    const char *message,
    void *storage);

MD_DECL(stdout, stdout_init,
    MD_DECL_ARGS(
        MD_ARG_DECL("serial", MD_ARG_MODE_NORMAL, MD_ARG_TYPE_PERIPHERAL, PP_SERIAL)
    )
);

int stdout_init(
    const md_decl_t *md,
    const char *name,
    md_arg_t *args)
{
    stdout_t *out_if;

    out_if = pvPortMalloc(sizeof(stdout_t));
    if (out_if == NULL) {
        return -1;
    }

    out_if->iface = IF_SERIAL(args[0].iface);

    out_if->iface->configure(out_if->iface,
        &(const if_serial_cf_t) {
        .baudrate = 1000000,
        .tx_buf_size = STDOUT_LINE_LENGTH,
        .rx_buf_size = 0,
        .flags = 0
    });

    log_register(stdout_log_handler, out_if);

    return 0;
}

static void stdout_log_handler(
    TaskHandle_t task,
    const char *message,
    void *storage)
{
    stdout_t *out_if = storage;
    int len;

    /* Make sure the output is ready... */

    len = tfp_snprintf(out_if->buffer, STDOUT_LINE_LENGTH, "%-16s: %s\n", pcTaskGetName(task), message);
    if (len >= STDOUT_LINE_LENGTH - 1) {
        len = STDOUT_LINE_LENGTH - 1;
    }
    out_if->iface->tx_write(out_if->iface, out_if->buffer, len);
}
