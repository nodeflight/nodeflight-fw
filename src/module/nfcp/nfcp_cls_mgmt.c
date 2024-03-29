/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2023  Max Sikström
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

#include "module/nfcp/nfcp_cls_mgmt.h"
#include "module/nfcp/nfcp.h"

#include "core/log.h"

#include "lib/pack.h"

#define NFCP_SESSION_ID_NONE       0x00000000

#define NFCP_CLS_MGMT_DEBUG        0

#if NFCP_CLS_MGMT_DEBUG
#pragma message "NFCP_CLS_MGMT_DEBUG defined"
#define D_PRINTLN(...) log_println(__VA_ARGS__);
#else
#define D_PRINTLN(...) do {} while(0)
#endif

typedef struct nfcp_cls_mgmt_s nfcp_cls_mgmt_t;

struct nfcp_cls_mgmt_s {
    uint32_t session_id;

};

static void *nfcp_cls_mgmt_init(
    nfcp_t *nfcp);

static void nfcp_cls_mgmt_reset(
    nfcp_t *nfcp,
    void *cls_storage);

static nfcp_op_status_t nfcp_cls_op_session_id(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length);

static nfcp_op_status_t nfcp_cls_op_log_message(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length);

static nfcp_op_status_t nfcp_cls_op_invalid_class(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length);

static nfcp_op_status_t nfcp_cls_op_invalid_operation(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length);

static void nfcp_cls_mgmt_log_handler(
    TaskHandle_t task,
    TickType_t tick,
    const char *message,
    void *storage);

const static nfcp_operation_t nfcp_cls_mgmt_ops[NFCP_CLS_MGMT_NUM_OPS] = {
    [NFCP_CLS_MGMT_OP_SESSION_ID] = {
        .handler = nfcp_cls_op_session_id,
        .handle_call = true
    },
    [NFCP_CLS_MGMT_OP_LOG_MESSAGE] = {
        .handler = nfcp_cls_op_log_message,
        .handle_info = true
    },
    [NFCP_CLS_MGMT_OP_INVALID_CLASS] = {
        .handler = nfcp_cls_op_invalid_class,
        .handle_info = true
    },
    [NFCP_CLS_MGMT_OP_INVALID_OPERATION] = {
        .handler = nfcp_cls_op_invalid_operation,
        .handle_info = true
    }
};

const nfcp_cls_t nfcp_cls_mgmt = {
    .cls_id = NFCP_CLS_MGMT,
    .init = nfcp_cls_mgmt_init,
    .reset = nfcp_cls_mgmt_reset,
    .num_ops = NFCP_CLS_MGMT_NUM_OPS,
    .ops = nfcp_cls_mgmt_ops
};

void *nfcp_cls_mgmt_init(
    nfcp_t *nfcp)
{
    nfcp_cls_mgmt_t *mgmt = pvPortMalloc(sizeof(nfcp_cls_mgmt_t));
    if (mgmt == NULL) {
        /* TODO: Error handling */
        return NULL;
    }

    log_register(nfcp_cls_mgmt_log_handler, nfcp);
    return mgmt;
}

void nfcp_cls_mgmt_reset(
    nfcp_t *nfcp,
    void *cls_storage)
{
    nfcp_cls_mgmt_t *mgmt = cls_storage;
    mgmt->session_id = NFCP_SESSION_ID_NONE;
    D_PRINTLN("mgmt: reset session");
}

nfcp_op_status_t nfcp_cls_op_session_id(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length)
{
    nfcp_cls_mgmt_t *mgmt = cls_storage;
    bool send_version;
    if (length != 4 || !is_call) {
        return NFCP_OP_STATUS_ERROR;
    }

    uint32_t pkt_session_id = pack_to_u32(payload);
    if (pkt_session_id == NFCP_SESSION_ID_NONE) {
        return NFCP_OP_STATUS_ERROR;
    }

    if (mgmt->session_id != pkt_session_id) {
        /* Reset session if not already reset */
        if (mgmt->session_id != NFCP_SESSION_ID_NONE) {
            nfcp_reset(nfcp);
        }
        /* Store session id */
        mgmt->session_id = pkt_session_id;

        /* Response with version */
        D_PRINTLN("mgmt: new session");

        send_version = true;
    } else {
        /* Response with empty */
        D_PRINTLN("mgmt: refresh session");
        send_version = false;
    }

    if (0 == nfcp_tx_take(nfcp)) {
        int resp_len = 0;
        nfcp->tx_buffer[resp_len++] = NFCP_CLS_MGMT << 2 | NFCP_HDR_BIT_IS_CALL | NFCP_HDR_BIT_IS_RESP;
        nfcp->tx_buffer[resp_len++] = NFCP_CLS_MGMT_OP_SESSION_ID;
        nfcp->tx_buffer[resp_len++] = seq_nr;
        if (send_version) {
            const char version_tag[] = "NodeFlight test";
            const char *cur;
            for (cur = version_tag; *cur; cur++) {
                nfcp->tx_buffer[resp_len++] = (uint8_t) *cur;
            }
        }
        nfcp_tx_packet(nfcp, nfcp->tx_buffer, resp_len);
        nfcp_tx_give(nfcp);
    }

    nfcp_refresh_session(nfcp);
    return NFCP_OP_STATUS_SUCCESS;
}

nfcp_op_status_t nfcp_cls_op_log_message(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length)
{
    /*
     * Nothing that can be done from FW point of view. Accept to not send a status back. Normal logging would be to write a log message to the client, which can lead to an infinite loop
     */
    return NFCP_OP_STATUS_SUCCESS;
}

nfcp_op_status_t nfcp_cls_op_invalid_class(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length)
{
    /* Nothing that can be done from FW point of view. Accept to not send a status back */
    return NFCP_OP_STATUS_SUCCESS;
}

nfcp_op_status_t nfcp_cls_op_invalid_operation(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length)
{
    /* Nothing that can be done from FW point of view. Accept to not send a status back */
    return NFCP_OP_STATUS_SUCCESS;
}

void nfcp_cls_mgmt_log_handler(
    TaskHandle_t task,
    TickType_t tick,
    const char *message,
    void *storage)
{
    nfcp_t *nfcp = storage;
    nfcp_cls_mgmt_t *mgmt = nfcp->class_storage[NFCP_CLS_MGMT];
    const char *task_name;

    if (mgmt->session_id != NFCP_SESSION_ID_NONE) {
        if (0 == nfcp_tx_take(nfcp)) {
            int len = 0;
            /* Header */
            nfcp->tx_buffer[len++] = NFCP_CLS_MGMT << 2;
            nfcp->tx_buffer[len++] = NFCP_CLS_MGMT_OP_LOG_MESSAGE;

            /* log context */
            nfcp->tx_buffer[len++] = 0;

            /* TODO: Better packing */
            /* TODO: Include Tick */

            /* message - task name */
            task_name = pcTaskGetName(task);
            while (*task_name != '\0' && len < NFCP_MAX_PACKET_LENGTH - 1) {
                nfcp->tx_buffer[len++] = *(task_name++);
            }

            /* message - field delimiter */
            nfcp->tx_buffer[len++] = '\t';

            /* message - message */
            while (*message != '\0' && len < NFCP_MAX_PACKET_LENGTH) {
                nfcp->tx_buffer[len++] = *(message++);
            }

            nfcp_tx_packet(nfcp, nfcp->tx_buffer, len);
            nfcp_tx_give(nfcp);
        }
    }
}
