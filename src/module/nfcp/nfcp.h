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

#pragma once

#include <stdint.h>
#include "core/interface_types.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define NFCP_MAX_PACKET_LENGTH        128
/* Add 16bit CRC + frame delimiter */
#define NFCP_PACKET_BUFFER_SIZE       (NFCP_MAX_PACKET_LENGTH * 2 + 3)

#define NFCP_MAX_CLASSES              4

#define NFCP_HDR_BIT_IS_CALL          0x02
#define NFCP_HDR_BIT_IS_RESP          0x01

typedef struct nfcp_s nfcp_t;
typedef struct nfcp_operation_s nfcp_operation_t;
typedef struct nfcp_cls_s nfcp_cls_t;

typedef enum nfcp_op_status_s {
    NFCP_OP_STATUS_SUCCESS = 0,
    NFCP_OP_STATUS_INVALID_OP,
    NFCP_OP_STATUS_ERROR
} nfcp_op_status_t;

typedef nfcp_op_status_t (*nfcp_operation_handler_t)(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length);

typedef void *(*nfcp_cls_init_t)(
    nfcp_t *nfcp);

typedef void (*nfcp_cls_reset_t)(
    nfcp_t *nfcp,
    void *cls_storage);

struct nfcp_s {
    if_serial_t *if_ser;
    TaskHandle_t task;
    TickType_t session_end_time;

    /* Recursive sempaphore. Can be taken from the same task prior to transmission, to also guard tx_buffer */
    SemaphoreHandle_t tx_mutex;

    uint8_t rx_buffer[NFCP_PACKET_BUFFER_SIZE];
    uint8_t tx_buffer[NFCP_PACKET_BUFFER_SIZE];

    void *class_storage[NFCP_MAX_CLASSES];
};

struct nfcp_operation_s {
    nfcp_operation_handler_t handler;
    bool handle_call;
    bool handle_info;
};

struct nfcp_cls_s {
    uint8_t cls_id;
    nfcp_cls_init_t init;
    nfcp_cls_reset_t reset;
    int num_ops;
    const nfcp_operation_t *ops;
};

/**
 * Send a packet
 *
 * @return 0 on success, negative on error
 */
int nfcp_tx_packet(
    nfcp_t *nfcp,
    uint8_t *buf,
    int len);

/**
 * Send a abort sequence
 *
 * Send a sequence to abort the packet and get the receiver into known state for accepting a new packet
 *
 * @return 0 on success, negative on error
 */
int nfcp_tx_abort(
    nfcp_t *nfcp);

/**
 * Take TX mutex
 */
int nfcp_tx_take(
    nfcp_t *nfcp);

/**
 * Give TX mutex
 */
void nfcp_tx_give(
    nfcp_t *nfcp);

/**
 * Reset a NFCP connection
 *
 * Resetting the connection clears all ongoing operations and reset the current session ID
 */
void nfcp_reset(
    nfcp_t *nfcp);

/**
 * Refresh NFCP session timeout
 *
 * Needs to be called within 5 seconds to keep NFCP session active. If not, the session will be reset
 *
 * Intended to be called every second
 */
void nfcp_refresh_session(
    nfcp_t *nfcp);

/**
 * Update header in tx_buf
 *
 * Assumes rx_buf contains a valid call request
 *
 * @return length of header
 */
static inline int nfcp_set_header(
    uint8_t *tx_buf,
    uint8_t cls,
    uint8_t op,
    bool is_call,
    bool is_resp,
    uint8_t seq_nr)
{
    tx_buf[0] = (cls << 2) | (is_call ? NFCP_HDR_BIT_IS_CALL : 0) | (is_resp ? NFCP_HDR_BIT_IS_RESP : 0);
    tx_buf[1] = op;
    if (is_call) {
        tx_buf[2] = seq_nr;
        return 3;
    } else {
        return 2;
    }
}
