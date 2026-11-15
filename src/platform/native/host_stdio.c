/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) Max Sikström
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

/*
 * host_stdio - a serial port on the standard output of the process
 *
 * Writes go to file descriptor 1. Nothing is ever received: a read waits for
 * its timeout and returns no data. It has no resources:
 *
 *   per console host_stdio
 *   mod - stdout console
 */

#include "core/peripheral.h"
#include "core/interface.h"
#include "core/interface_types.h"

#include "FreeRTOS.h"
#include "task.h"

#include <errno.h>
#include <stddef.h>
#include <unistd.h>

#define HOST_STDIO_FD 1

typedef struct host_stdio_if_s host_stdio_if_t;

struct host_stdio_if_s {
    if_serial_t header;
};

static int host_stdio_init(
    if_header_t *iface);

static int host_stdio_configure(
    if_serial_t *iface,
    const if_serial_cf_t *config);

static int host_stdio_tx_write(
    if_serial_t *iface,
    const void *buf,
    int bytes);

static void host_stdio_tx_wait_done(
    if_serial_t *iface,
    TickType_t timeout);

static int host_stdio_rx_read(
    if_serial_t *iface,
    uint8_t *dst,
    int dst_size,
    TickType_t timeout);

PP_TYPE_DECL(host_stdio, PP_SERIAL, 0, host_stdio_init, sizeof(host_stdio_if_t));

PP_INST_DECL(host_stdio, host_stdio, NULL,
    PP_INST_RS_TERMINATION
);

int host_stdio_init(
    if_header_t *iface)
{
    host_stdio_if_t *if_stdio = (host_stdio_if_t *) iface;

    if_stdio->header.configure = host_stdio_configure;
    if_stdio->header.tx_write = host_stdio_tx_write;
    if_stdio->header.tx_wait_done = host_stdio_tx_wait_done;
    if_stdio->header.rx_read = host_stdio_rx_read;

    return 0;
}

int host_stdio_configure(
    if_serial_t *iface,
    const if_serial_cf_t *config)
{
    return 0;
}

/**
 * Write all bytes to the standard output
 *
 * Retries when interrupted by a signal, such as the tick of FreeRTOS, and
 * after partial writes.
 *
 * @return the number of bytes written, or -1 on error
 */
int host_stdio_tx_write(
    if_serial_t *iface,
    const void *buf,
    int bytes)
{
    const char *cur = buf;
    int remaining = bytes;
    ssize_t len;

    while (remaining > 0) {
        len = write(HOST_STDIO_FD, cur, (size_t) remaining);
        if (len < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        cur += len;
        remaining -= (int) len;
    }
    return bytes;
}

void host_stdio_tx_wait_done(
    if_serial_t *iface,
    TickType_t timeout)
{
    /* Writes are synchronous, nothing to wait for */
}

int host_stdio_rx_read(
    if_serial_t *iface,
    uint8_t *dst,
    int dst_size,
    TickType_t timeout)
{
    /* Nothing is received: wait out the timeout, so a reader does not spin */
    vTaskDelay(timeout);
    return 0;
}
