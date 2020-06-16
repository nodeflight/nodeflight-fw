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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define NFCP_MAX_PACKET_LENGTH        256

/* Add 16bit CRC + frame delimiter */
#define NFCP_PACKET_BUFFER_SIZE       (NFCP_MAX_PACKET_LENGTH * 2 + 3)

#define NFCP_IF_BUFFER_SIZE           128

#define NFCP_TASK_PRIORITY            2
#define NFCP_TASK_STACK_WORDS         512

#define NFCP_SERIAL_BAUDRATE          230400

typedef struct nfcp_s nfcp_t;

struct nfcp_s {
    if_serial_t *if_ser;
    TaskHandle_t task;
};

static int nfcp_init(
    const char *name,
    md_arg_t *args);

static void nfcp_tx_packet(
    nfcp_t *nfcp,
    uint8_t *buf,
    int len);

static int nfcp_rx_packet(
    nfcp_t *nfcp,
    uint8_t *buf,
    int size);

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
    uint16_t crc = crc16(buf, len - 2, 0, CRC16_POLY_CCITT);
    len -= 2;
    if (buf[len] != ((crc >> 8) & 0xff) || buf[len + 1] != ((crc >> 0) & 0xff)) {
        return -1;
    }
    return len;
}

void nfcp_task(
    void *storage)
{
    nfcp_t *nfcp = storage;
    uint8_t packet[NFCP_PACKET_BUFFER_SIZE];
    int len;

    /* Just for test, keep a packet that contains escaped data */
    packet[0] = 0x00;
    packet[1] = 0x17;
    packet[2] = 0x02;
    packet[3] = 0x03;
    nfcp_tx_packet(nfcp, packet, 4);

    for (;;) {
        len = nfcp_rx_packet(nfcp, packet, NFCP_PACKET_BUFFER_SIZE);
        if (len < 0) {
            log_println("Unknown packet");
        } else {
            log_println("Valid packet len=%d", len);

            packet[0] ^= 0x80;
            len = hdlc_frame_stuff(packet, len);
            nfcp->if_ser->tx_write(nfcp->if_ser, packet, len);
        }
    }
}
