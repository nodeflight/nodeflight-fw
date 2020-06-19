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
#include "lib/crc.h"
#include "lib/hdlc.h"
#include "FreeRTOS.h"
#include "task.h"

#include "core/log.h"
#include "vendor/tinyprintf/tinyprintf.h"

#include "module/nfcp/nfcp.h"

#include "module/nfcp/nfcp_cls_mgmt.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define NFCP_IF_BUFFER_SIZE           128

#define NFCP_TASK_PRIORITY            2
#define NFCP_TASK_STACK_WORDS         256

#define NFCP_SERIAL_BAUDRATE          230400

#define NFCP_DEBUG                    0
#define NFCP_DEBUG_DISABLE_CRC_CHECK  0

#if NFCP_DEBUG
#pragma message "NFCP_DEBUG defined"
#define D_PRINTLN(...) log_println(__VA_ARGS__);
#else
#define D_PRINTLN(...) do {} while(0)
#endif

/**
 * Disable incoming CRC check. This should never be checked in with 1. Used to simplify testing using a common serial
 * terminal in hex mode.
 */

static int nfcp_init(
    const char *name,
    md_arg_t *args);

static int nfcp_rx_packet(
    nfcp_t *nfcp,
    uint8_t *buf,
    int size);

static void nfcp_task(
    void *storage);

MD_DECL(nfcp, "p", nfcp_init);

static const nfcp_cls_t *const nfcp_class[NFCP_MAX_CLASSES] = {
    [NFCP_CLS_MGMT] = &nfcp_cls_mgmt
};

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
        .tx_buf_size = NFCP_IF_BUFFER_SIZE,
        .rx_buf_size = NFCP_IF_BUFFER_SIZE,
        .flags = IF_SERIAL_HAS_FRAME_DELIMITER,
        .frame_delimiter = HDLC_FRAME_BOUNDARY
    });

    /* Generate traceable name for debug/stats */
    char taskname[configMAX_TASK_NAME_LEN];
    tfp_snprintf(taskname, configMAX_TASK_NAME_LEN, "md nfcp %s", name == NULL ? "-" : name);

    xTaskCreate(nfcp_task, taskname, NFCP_TASK_STACK_WORDS, nfcp, NFCP_TASK_PRIORITY, &nfcp->task);
    return 0;
}

void nfcp_tx_packet(
    nfcp_t *nfcp,
    uint8_t *buf,
    int len)
{
    uint16_t crc = crc16(buf, len, 0, CRC16_POLY_CCITT);
    buf[len++] = (crc >> 8) & 0xff;
    buf[len++] = (crc >> 0) & 0xff;
    len = hdlc_frame_stuff(buf, len);
    nfcp->if_ser->tx_write(nfcp->if_ser, buf, len);
}

int nfcp_rx_packet(
    nfcp_t *nfcp,
    uint8_t *buf,
    int size)
{
    int len;
    len = 0;
    while (len < size && (len == 0 || buf[len - 1] != HDLC_FRAME_BOUNDARY)) {
        len += nfcp->if_ser->rx_read(nfcp->if_ser, buf + len, size - len, portMAX_DELAY);
    }
    len = hdlc_frame_unstuff(buf, len);
    if (len < 2) {
        return -1;
    }
#if NFCP_DEBUG_DISABLE_CRC_CHECK
#pragma message ( "NFCP_DEBUG_DISABLE_CRC_CHECK is enabled" )
    len -= 2;
#else
    uint16_t crc = crc16(buf, len - 2, 0, CRC16_POLY_CCITT);
    len -= 2;
    if (buf[len] != ((crc >> 8) & 0xff) || buf[len + 1] != ((crc >> 0) & 0xff)) {
        return -1;
    }
#endif
    return len;
}

void nfcp_task(
    void *storage)
{
    nfcp_t *nfcp = storage;
    int len;

    int i;

    /* Initialize classes */
    for (i = 0; i < NFCP_MAX_CLASSES; i++) {
        if (nfcp_class[i] != NULL) {
            nfcp->class_storage[i] = nfcp_class[i]->init(nfcp);
        } else {
            nfcp->class_storage[i] = NULL;
        }
    }

    nfcp_reset(nfcp);

    for (;;) {
        len = nfcp_rx_packet(nfcp, nfcp->buffer, NFCP_PACKET_BUFFER_SIZE);
        if (len < 0) {
            D_PRINTLN("Unknown packet");
        } else if (len < 2 || ((nfcp->buffer[0] & 0x02) && len < 3)) {
            D_PRINTLN("No header");
        } else {
            uint8_t class = (nfcp->buffer[0] & 0xfc) >> 2;
            uint8_t operation = nfcp->buffer[1];
            bool is_call = (nfcp->buffer[0] & 0x02) != 0;
            bool is_resp = (nfcp->buffer[0] & 0x01) != 0;
            uint8_t seq_nr = 0;

            uint8_t *payload;
            int payload_len;

            if (is_call) {
                seq_nr = nfcp->buffer[2];
                payload_len = len - 3;
                payload = nfcp->buffer + 3;
            } else {
                payload_len = len - 2;
                payload = nfcp->buffer + 2;
            }

            if (class >= NFCP_MAX_CLASSES || nfcp_class[class] == NULL) {
                D_PRINTLN("Unknown class");
                nfcp->buffer[0] = NFCP_CLS_MGMT << 2;
                nfcp->buffer[1] = NFCP_CLS_MGMT_OP_INVALID_CLASS;
                nfcp->buffer[2] = class << 2 | (is_call ? 0x02 : 0) | (is_resp ? 0x01 : 0);
                nfcp->buffer[3] = operation;
                nfcp->buffer[4] = seq_nr;
                nfcp_tx_packet(nfcp, nfcp->buffer, 5);
            } else if (operation >= nfcp_class[class]->num_ops
                       || nfcp_class[class]->ops[operation].handler == NULL
                       || (is_call && !nfcp_class[class]->ops[operation].handle_call)
                       || (!is_call && !nfcp_class[class]->ops[operation].handle_info)
            ) {
                D_PRINTLN("Unknown operation");
                nfcp->buffer[0] = NFCP_CLS_MGMT << 2;
                nfcp->buffer[1] = NFCP_CLS_MGMT_OP_INVALID_OPERATION;
                nfcp->buffer[2] = class << 2 | (is_call ? 0x02 : 0) | (is_resp ? 0x01 : 0);
                nfcp->buffer[3] = operation;
                nfcp->buffer[4] = seq_nr;
                nfcp_tx_packet(nfcp, nfcp->buffer, 5);
            } else {
                /* Valid request */
                D_PRINTLN("Valid packet class=%u op=%u call=%d resp=%d len=%d",
                    class,
                    operation,
                    is_call,
                    is_resp,
                    payload_len);

                nfcp_class[class]->ops[operation].handler(nfcp,
                    nfcp->class_storage[class],
                    is_call,
                    seq_nr,
                    payload,
                    payload_len);
            }
        }
    }
}

void nfcp_reset(
    nfcp_t *nfcp)
{
    int i;
    for (i = 0; i < NFCP_MAX_CLASSES; i++) {
        if (nfcp_class[i] != NULL) {
            nfcp_class[i]->reset(nfcp, nfcp->class_storage[i]);
        }
    }
}
