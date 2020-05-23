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
#include "core/scheduler.h"
#include "core/variable.h"
#include "FreeRTOS.h"
#include "task.h"

#include "vendor/tinyprintf/tinyprintf.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define FPORT_FLAG_FAILSAFE   0x08
#define FPORT_FLAG_FRAME_LOST 0x04
#define FPORT_FLAG_CH18       0x02
#define FPORT_FLAG_CH17       0x04

#define FPORT_TASK_PRIORITY   15

#define FPORT_TIMEOUT         (50 * portTICK_PERIOD_MS)

#define FPORT_NOTIFY_PACKET   (1 << 0)

#define FPORT_raw_chn_MIN     192
#define FPORT_raw_chn_MAX     1792

#define FPORT_STACK_WORDS     128

typedef struct fport_s fport_t;

struct fport_s {
    if_header_t *if_ser;
    sc_t *target_scheduler;
    TaskHandle_t task;

    /* Sync from IRQ->thread */
    uint16_t raw_chn[16];
    uint8_t flags;
    uint8_t rssi;

    /* Output variables */
    float f_chn[18];
    float f_rssi;
    bool failsafe;
    bool signal_loss;
};

static int fport_init(
    const char *name,
    md_arg_t *args);

static void fport_task(
    void *storage);

MD_DECL(fport, "p?s", fport_init);

#define GET_ESC_CHAR(_TGT) \
    do { \
        if (*src_len == 0) { \
            return -1; \
        } \
        _TGT = **src; \
        (*src)++; \
        (*src_len)--; \
        if (_TGT == 0x7d) { \
            if (*src_len == 0) { \
                return -1; \
            } \
            _TGT = 0x20 ^ (**src); \
            (*src)++; \
            (*src_len)--; \
        } \
    } while(0)

static int fport_unpack_packet(
    uint8_t *dst,
    uint16_t dst_len,
    const uint8_t **src,
    uint16_t *src_len)
{
    int len = -1;
    int i;
    uint8_t c;
    uint16_t checksum;
    /* Find start of frame */
    while (**src != 0x7e) {
        if (*src_len > 0) {
            (*src)++;
            (*src_len)--;
        } else {
            return -1;
        }
    }
    /* Skip start of frame */
    if (*src_len == 0) {
        return -1;
    }
    (*src)++;
    (*src_len)--;

    /* Get len */
    GET_ESC_CHAR(len);
    checksum = len;
    for (i = 0; i < len; i++) {
        GET_ESC_CHAR(c);
        checksum += c;

        if (i >= dst_len) {
            return -4;
        }
        dst[i] = c;
    }

    /* Fetch checksum */
    GET_ESC_CHAR(c);
    checksum += c;
    while (checksum > 0xff) {
        checksum = (checksum & 0xff) + (checksum >> 8);
    }
    if (checksum != 0xff) {
        return -3;
    }

    /* Strip end of frame */
    if (**src != 0x7e) {
        return -2;
    }
    if (*src_len > 0) {
        (*src)++;
        (*src_len)--;
    } else {
        return -1;
    }

    return len;
}

static void fport_rx_done(
    const uint8_t *buf,
    uint16_t len,
    void *storage)
{
    uint8_t packet[64];
    int packet_len;

    fport_t *fport_if = storage;

    /* If a frame is received, but contains no data, assume failsafe */
    fport_if->flags = FPORT_FLAG_FAILSAFE;

    while ((packet_len = fport_unpack_packet(packet, sizeof(packet), &buf, &len)) > 0) {
        if (packet[0] == 0x00 && packet_len == 25) {
            /* Control frame */
            fport_if->raw_chn[0] = 0x07ffU & (((uint16_t) packet[1]) >> 0 | ((uint16_t) packet[2]) << 8);
            fport_if->raw_chn[1] = 0x07ffU & (((uint16_t) packet[2]) >> 3 | ((uint16_t) packet[3]) << 5);
            fport_if->raw_chn[2] = 0x07ffU
                & (((uint16_t) packet[3]) >> 6 | ((uint16_t) packet[4]) << 2 | ((uint16_t) packet[5]) << 10);
            fport_if->raw_chn[3] = 0x07ffU & (((uint16_t) packet[5]) >> 1 | ((uint16_t) packet[6]) << 7);
            fport_if->raw_chn[4] = 0x07ffU & (((uint16_t) packet[6]) >> 4 | ((uint16_t) packet[7]) << 4);
            fport_if->raw_chn[5] = 0x07ffU
                & (((uint16_t) packet[7]) >> 7 | ((uint16_t) packet[8]) << 1 | ((uint16_t) packet[9]) << 9);
            fport_if->raw_chn[6] = 0x07ffU & (((uint16_t) packet[9]) >> 2 | ((uint16_t) packet[10]) << 6);
            fport_if->raw_chn[7] = 0x07ffU & (((uint16_t) packet[10]) >> 5 | ((uint16_t) packet[11]) << 3);

            fport_if->raw_chn[8] = 0x07ffU & (((uint16_t) packet[12]) >> 0 | ((uint16_t) packet[13]) << 8);
            fport_if->raw_chn[9] = 0x07ffU & (((uint16_t) packet[13]) >> 3 | ((uint16_t) packet[14]) << 5);
            fport_if->raw_chn[10] = 0x07ffU
                & (((uint16_t) packet[14]) >> 6 | ((uint16_t) packet[15]) << 2 | ((uint16_t) packet[16]) << 10);
            fport_if->raw_chn[11] = 0x07ffU & (((uint16_t) packet[16]) >> 1 | ((uint16_t) packet[17]) << 7);
            fport_if->raw_chn[12] = 0x07ffU & (((uint16_t) packet[17]) >> 4 | ((uint16_t) packet[18]) << 4);
            fport_if->raw_chn[13] = 0x07ffU
                & (((uint16_t) packet[18]) >> 7 | ((uint16_t) packet[19]) << 1 | ((uint16_t) packet[20]) << 9);
            fport_if->raw_chn[14] = 0x07ffU & (((uint16_t) packet[20]) >> 2 | ((uint16_t) packet[21]) << 6);
            fport_if->raw_chn[15] = 0x07ffU & (((uint16_t) packet[21]) >> 5 | ((uint16_t) packet[22]) << 3);

            fport_if->flags = packet[23];
            fport_if->rssi = packet[24];
        } else if (packet[0] == 0x01) {
            /* Downlink data */
            /* TODO: add telemetry */
        }
    }

    BaseType_t should_switch = pdFALSE;
    xTaskNotifyFromISR(fport_if->task, FPORT_NOTIFY_PACKET, eSetBits, &should_switch);
    portYIELD_FROM_ISR(should_switch);
}

int fport_init(
    const char *name,
    md_arg_t *args)
{
    fport_t *fport_if;
    int i;

    if (args[0].iface->peripheral->decl->type != PP_SERIAL) {
        return -1;
    }

    fport_if = pvPortMalloc(sizeof(fport_t));
    if (fport_if == NULL) {
        return -1;
    }

    for (i = 0; i < 16; i++) {
        fport_if->raw_chn[i] = 0;
    }
    fport_if->rssi = 0;
    fport_if->flags = FPORT_FLAG_FAILSAFE;
    fport_if->if_ser = args[0].iface;
    fport_if->target_scheduler = args[1].sched;

    /* initialize output channels */
    for (i = 0; i < 18; i++) {
        fport_if->f_chn[i] = 0.0f;
    }
    fport_if->f_rssi = 0.0;
    fport_if->failsafe = true;
    fport_if->signal_loss = true;

    if (fport_if->target_scheduler != NULL) {
        if (0 != sc_configure_source(fport_if->target_scheduler, 0.009f)) {
            return -1;
        }
    }

    xTaskCreate(fport_task,
        "fport_proc",
        FPORT_STACK_WORDS,
        fport_if,
        FPORT_TASK_PRIORITY,
        &fport_if->task);

    IF_SERIAL(fport_if->if_ser)->configure(IF_SERIAL(fport_if->if_ser),
        &(const if_serial_cf_t) {
        .baudrate = 115200,
        .tx_buf_size = 16,
        .rx_buf_size = 128,
        .flags = (
            IF_SERIAL_HALF_DUPLEX
            | IF_SERIAL_INVERTED_RX
            | IF_SERIAL_INVERTED_TX
            ),
        .rx_done = fport_rx_done,
        .storage = fport_if
    });

    vr_register(name, "ffffffffffffffffffffbb",
        &fport_if->f_chn[0], &fport_if->f_chn[1], &fport_if->f_chn[2], &fport_if->f_chn[3],
        &fport_if->f_chn[4], &fport_if->f_chn[5], &fport_if->f_chn[6], &fport_if->f_chn[7],
        &fport_if->f_chn[8], &fport_if->f_chn[9], &fport_if->f_chn[10], &fport_if->f_chn[11],
        &fport_if->f_chn[12], &fport_if->f_chn[13], &fport_if->f_chn[14], &fport_if->f_chn[15],
        &fport_if->f_chn[16], &fport_if->f_chn[17], &fport_if->f_rssi,
        &fport_if->failsafe, &fport_if->signal_loss);

    return 0;
}

void fport_task(
    void *storage)
{
    fport_t *fport_if = storage;
    uint32_t notify_value;
    int i;

    for (;;) {
        xTaskNotifyWait(0x00, UINT32_MAX, &notify_value, FPORT_TIMEOUT);
        if (notify_value & FPORT_NOTIFY_PACKET) {
            if (fport_if->flags & FPORT_FLAG_FAILSAFE) {
                /* Failsafe */
                fport_if->failsafe = true;
                fport_if->signal_loss = true;
            } else if (fport_if->flags & FPORT_FLAG_FRAME_LOST) {
                /* Frame lost */
                fport_if->signal_loss = true;
            } else {
                /* Successful RX */
                for (i = 0; i < 16; i++) {
                    /* Map to range 0.0f to 1.0f */
                    float val = (float) (fport_if->raw_chn[i] - FPORT_raw_chn_MIN)
                        / (FPORT_raw_chn_MAX - FPORT_raw_chn_MIN);
                    /* Extend to range -1.0f to 1.0f */
                    fport_if->f_chn[i] = val * 2.0f - 1.0f;
                }
                fport_if->f_chn[17] = (fport_if->flags & FPORT_FLAG_CH17) ? 1.0f : -1.0f;
                fport_if->f_chn[18] = (fport_if->flags & FPORT_FLAG_CH18) ? 1.0f : -1.0f;
                fport_if->f_rssi = (float) fport_if->rssi;
                fport_if->failsafe = false;
                fport_if->signal_loss = false;
            }
        } else {
            /* Timeout */
            fport_if->failsafe = true;
            fport_if->signal_loss = true;
        }
        if (fport_if->target_scheduler != NULL) {
            sc_trigger(fport_if->target_scheduler);
        }
    }
}
