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
 * Nodeflight control port
 *
 * generic port for system management
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

#define MAX_PACKET_LENGTH             256

#define NFCP_TASK_PRIORITY            2
#define NFCP_TASK_STACK_WORDS         256

#define NFCP_SERIAL_BAUDRATE          115200

typedef struct nfcp_s nfcp_t;

struct nfcp_s {
    if_serial_t *if_ser;
    TaskHandle_t task;
};

static int nfcp_init(
    const char *name,
    md_arg_t *args);

static void nfcp_task(
    void *storage);

MD_DECL(nfcp, "p", nfcp_init);

int nfcp_init(
    const char *name,
    md_arg_t *args)
{
    if (args[0].iface->peripheral->decl->type != PP_SERIAL) {
        return -1;
    }

    nfcp_t *nfcp;

    nfcp = pvPortMalloc(sizeof(nfcp_t));
    if (nfcp == NULL) {
        return -1;
    }

    nfcp->if_ser = IF_SERIAL(args[0].iface);

    nfcp->if_ser->configure(nfcp->if_ser,
        &(const if_serial_cf_t) {
        .baudrate = NFCP_SERIAL_BAUDRATE,
        .tx_buf_size = MAX_PACKET_LENGTH,
        .rx_buf_size = MAX_PACKET_LENGTH,
        .flags = IF_SERIAL_HAS_FRAME_DELIMITER,
        .frame_delimiter = '\n'
    });

    /* Generate traceable name for debug/stats */
    char taskname[configMAX_TASK_NAME_LEN];
    tfp_snprintf(taskname, configMAX_TASK_NAME_LEN, "md nfcp %s", name);

    xTaskCreate(nfcp_task, taskname, NFCP_TASK_STACK_WORDS, nfcp, NFCP_TASK_PRIORITY, &nfcp->task);
    return 0;
}

void nfcp_task(
    void *storage)
{
    nfcp_t *nfcp = storage;
    uint8_t line[128];
    int len;
    int i;
    nfcp->if_ser->tx_write(nfcp->if_ser, "start\n", 6);
    for (;;) {
        len = 0;
        while (len < 128 && (len == 0 || line[len - 1] != '\n')) {
            len += nfcp->if_ser->rx_read(nfcp->if_ser, line+len, 128-len, portMAX_DELAY);
        }
        for (i = 0; i < len; i++) {
            if (line[i] >= 'a' && line[i] <= 'z') {
                line[i] += 'A' - 'a';
            }
        }
        tfp_printf("len=%d\n", len);
        nfcp->if_ser->tx_write(nfcp->if_ser, line, len);
    }
}
