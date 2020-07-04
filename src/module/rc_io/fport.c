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
#include "lib/hdlc.h"
#include "FreeRTOS.h"
#include "task.h"

#include "core/log.h"
#include "vendor/tinyprintf/tinyprintf.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define DEBUG_ERRORS          0

#if DEBUG_ERRORS
#define D_ERROR_PRINTLN(...) log_println(__VA_ARGS__);
#else
#define D_ERROR_PRINTLN(...) do {} while(0)
#endif

#define FPORT_FLAG_FAILSAFE        0x08
#define FPORT_FLAG_FRAME_LOST      0x04
#define FPORT_FLAG_CH18            0x02
#define FPORT_FLAG_CH17            0x04

#define FPORT_TIMEOUT              pdMS_TO_TICKS(50)

#define FPORT_RAW_VALUE_MIN        192
#define FPORT_RAW_VALUE_MAX        1792

/**
 * Buffer size packet processing
 *
 * It will be stored unstuffed in this buffer. Worst case is therefore double the size
 */
#define FPORT_RX_MAX_PACKET_SIZE   64
/**
 * Buffer size for uart rx queue
 * Note that this should handle padding, and reception of next packet prior to previous being processed
 *
 * Can be less than packet size, at the expense of more task switches
 */
#define FPORT_RX_BUFFER_SIZE       64
/**
 * Buffer size for uart tx queue
 * Should handle telemetry response, nothing more reall
 */
#define FPORT_TX_BUFFER_SIZE       32

#define FPORT_TASK_PRIORITY        7
#define FPORT_STACK_WORDS          256

typedef struct fport_s fport_t;

struct fport_s {
    if_serial_t *if_ser;
    sc_t *target_scheduler;
    TaskHandle_t task;

    /* Output variables */
    float f_chn[18];
    float f_rssi;
    bool failsafe;
    bool signal_loss;
};

static int fport_init(
    const md_decl_t *md,
    const char *name,
    md_arg_t *args);

static void fport_task(
    void *storage);

MD_DECL(fport, fport_init,
    MD_DECL_ARGS(
        MD_ARG_DECL("serial", MD_ARG_MODE_NORMAL, MD_ARG_TYPE_PERIPHERAL, PP_SERIAL),
        MD_ARG_DECL("schedule", MD_ARG_MODE_OPTIONAL, MD_ARG_TYPE_SCHEDULER, SC_DIR_OUT)
    ),
    MD_DECL_OUTPUTS(
        MD_OUTPUT_DECL("ch1", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch2", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch3", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch4", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch5", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch6", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch7", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch8", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch9", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch10", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch11", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch12", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch13", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch14", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch15", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch16", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("ch17", VR_TYPE_FLOAT), /* bool mapped to float */
        MD_OUTPUT_DECL("ch18", VR_TYPE_FLOAT), /* bool mapped to float */
        MD_OUTPUT_DECL("rssi", VR_TYPE_FLOAT),
        MD_OUTPUT_DECL("failsafe", VR_TYPE_BOOL),
        MD_OUTPUT_DECL("signal_loss", VR_TYPE_BOOL)
    )
);

int fport_init(
    const md_decl_t *md,
    const char *name,
    md_arg_t *args)
{
    fport_t *fport_if;
    int i;

    fport_if = pvPortMalloc(sizeof(fport_t));
    if (fport_if == NULL) {
        return -1;
    }

    fport_if->if_ser = IF_SERIAL(args[0].iface);
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

    fport_if->if_ser->configure(fport_if->if_ser,
        &(const if_serial_cf_t) {
        .baudrate = 115200,
        .tx_buf_size = FPORT_TX_BUFFER_SIZE,
        .rx_buf_size = FPORT_RX_BUFFER_SIZE,
        .flags = (
            IF_SERIAL_HALF_DUPLEX
            | IF_SERIAL_INVERTED_RX
            | IF_SERIAL_INVERTED_TX
            | IF_SERIAL_HAS_FRAME_DELIMITER
            ),
        .frame_delimiter = HDLC_FRAME_BOUNDARY,
        .storage = fport_if
    });

    vr_register(name, md->outputs,
        &fport_if->f_chn[0], &fport_if->f_chn[1], &fport_if->f_chn[2], &fport_if->f_chn[3],
        &fport_if->f_chn[4], &fport_if->f_chn[5], &fport_if->f_chn[6], &fport_if->f_chn[7],
        &fport_if->f_chn[8], &fport_if->f_chn[9], &fport_if->f_chn[10], &fport_if->f_chn[11],
        &fport_if->f_chn[12], &fport_if->f_chn[13], &fport_if->f_chn[14], &fport_if->f_chn[15],
        &fport_if->f_chn[16], &fport_if->f_chn[17], &fport_if->f_rssi,
        &fport_if->failsafe, &fport_if->signal_loss);

    /* Generate traceable name for debug/stats */
    char taskname[configMAX_TASK_NAME_LEN];
    tfp_snprintf(taskname, configMAX_TASK_NAME_LEN, "md fport %s", name == NULL ? "-" : name);

    xTaskCreate(fport_task,
        taskname,
        FPORT_STACK_WORDS,
        fport_if,
        FPORT_TASK_PRIORITY,
        &fport_if->task);

    return 0;
}

static float raw2float(
    uint16_t raw)
{
    int16_t signed_raw = 0x07ff & raw;
    /* Map to range 0.0f to 1.0f */
    float val = (float) (signed_raw - FPORT_RAW_VALUE_MIN)
        / (FPORT_RAW_VALUE_MAX - FPORT_RAW_VALUE_MIN);
    /* Extend to range -1.0f to 1.0f */
    return val * 2.0f - 1.0f;
}

void fport_task(
    void *storage)
{
    fport_t *fport_if = storage;

    uint8_t buf[FPORT_RX_MAX_PACKET_SIZE];
    uint16_t checksum;
    int res;
    int len;
    int i;

    bool channel_update;

    for (;;) {
        /* Reading is guaranteed to end with frame boundary to keep sync */
        len = 0;
        while (len == 0 || buf[len - 1] != HDLC_FRAME_BOUNDARY) {
            if (len == FPORT_RX_MAX_PACKET_SIZE) {
                /* TODO: Error handling. For now, wrap around, the packet will get bad checksum anyway */
                len = 0;
            }
            res = fport_if->if_ser->rx_read(fport_if->if_ser, &buf[len], FPORT_RX_MAX_PACKET_SIZE - len, FPORT_TIMEOUT);
            if (res < 0) {
                /* TODO: error handling */
            } else if (res == 0) {
                /* Timeout, force scheduler to trigger with loss */
                fport_if->failsafe = true;
                fport_if->signal_loss = true;
                D_ERROR_PRINTLN("timeout");
                if (fport_if->target_scheduler != NULL) {
                    sc_trigger(fport_if->target_scheduler);
                }
            } else {
                len += res;
            }
        }
        /* Unstuff bytes */
        len = hdlc_frame_unstuff(buf, len);

        /* Ignore error frames and empty frames */
        if (len <= 0) {
            continue;
        }

        /* Validate buf length against length header */
        if (len != buf[0] + 2) {
            /* Invalid length, drop */
            D_ERROR_PRINTLN("Invalid length %d %u", len, buf[0]);
            continue;
        }

        /* Calculate checksum */
        checksum = 0;
        for (i = 0; i < len; i++) {
            checksum += buf[i];
        }

        while (checksum > 0xff) {
            checksum = (checksum & 0xff) + (checksum >> 8);
        }
        if (checksum != 0xff) {
            /* Invalid checksum, drop */
            D_ERROR_PRINTLN("Invalid checksum %u", checksum);
            continue;
        }

        /* Parse packet */
        channel_update = false;
        if (buf[1] == 0x00 && len == 27) {
/* Control frame */
            fport_if->f_chn[0] = raw2float(((uint16_t) buf[2]) >> 0 | ((uint16_t) buf[3]) << 8);
            fport_if->f_chn[1] = raw2float(((uint16_t) buf[3]) >> 3 | ((uint16_t) buf[4]) << 5);
            fport_if->f_chn[2] =
                raw2float(((uint16_t) buf[4]) >> 6 | ((uint16_t) buf[5]) << 2 | ((uint16_t) buf[6]) << 10);
            fport_if->f_chn[3] = raw2float(((uint16_t) buf[6]) >> 1 | ((uint16_t) buf[7]) << 7);
            fport_if->f_chn[4] = raw2float(((uint16_t) buf[7]) >> 4 | ((uint16_t) buf[8]) << 4);
            fport_if->f_chn[5] =
                raw2float(((uint16_t) buf[8]) >> 7 | ((uint16_t) buf[9]) << 1 | ((uint16_t) buf[10]) << 9);
            fport_if->f_chn[6] = raw2float(((uint16_t) buf[10]) >> 2 | ((uint16_t) buf[11]) << 6);
            fport_if->f_chn[7] = raw2float(((uint16_t) buf[11]) >> 5 | ((uint16_t) buf[12]) << 3);
            fport_if->f_chn[8] = raw2float(((uint16_t) buf[13]) >> 0 | ((uint16_t) buf[14]) << 8);
            fport_if->f_chn[9] = raw2float(((uint16_t) buf[14]) >> 3 | ((uint16_t) buf[15]) << 5);
            fport_if->f_chn[10] =
                raw2float(((uint16_t) buf[15]) >> 6 | ((uint16_t) buf[16]) << 2 | ((uint16_t) buf[17]) << 10);
            fport_if->f_chn[11] = raw2float(((uint16_t) buf[17]) >> 1 | ((uint16_t) buf[18]) << 7);
            fport_if->f_chn[12] = raw2float(((uint16_t) buf[18]) >> 4 | ((uint16_t) buf[19]) << 4);
            fport_if->f_chn[13] =
                raw2float(((uint16_t) buf[19]) >> 7 | ((uint16_t) buf[20]) << 1 | ((uint16_t) buf[21]) << 9);
            fport_if->f_chn[14] = raw2float(((uint16_t) buf[21]) >> 2 | ((uint16_t) buf[22]) << 6);
            fport_if->f_chn[15] = raw2float(((uint16_t) buf[22]) >> 5 | ((uint16_t) buf[23]) << 3);
            fport_if->f_chn[16] = (buf[24] & FPORT_FLAG_CH17) ? 1.0f : -1.0f;
            fport_if->f_chn[17] = (buf[24] & FPORT_FLAG_CH18) ? 1.0f : -1.0f;
            fport_if->f_rssi = (float) buf[25];
            fport_if->failsafe = (buf[24] & FPORT_FLAG_FAILSAFE) ? true : false;
            fport_if->signal_loss = (buf[24] & FPORT_FLAG_FRAME_LOST) ? true : false;
            channel_update = true;
        } else if (buf[0] == 0x01) {
/* Downlink data */
/* TODO: add telemetry */
        }

        if (channel_update && fport_if->target_scheduler != NULL) {
            sc_trigger(fport_if->target_scheduler);
        }

    }
}
