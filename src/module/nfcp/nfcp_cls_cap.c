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

#include "module/nfcp/nfcp_cls_cap.h"
#include "module/nfcp/nfcp.h"
#include "lib/pack.h"

#include "core/resource.h"
#include "core/peripheral.h"
#include "core/module.h"

static const char *nf_target = NF_TARGET;

static void *nfcp_cls_cap_init(
    nfcp_t *nfcp);

static void nfcp_cls_cap_reset(
    nfcp_t *nfcp,
    void *cls_storage);

static nfcp_op_status_t nfcp_cls_cap_op_get_info(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length);

static nfcp_op_status_t nfcp_cls_cap_op_get_resource(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length);

static nfcp_op_status_t nfcp_cls_cap_op_get_peripheral(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length);

static nfcp_op_status_t nfcp_cls_cap_op_get_module(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length);

const static nfcp_operation_t nfcp_cls_cap_ops[NFCP_CLS_CAP_NUM_OPS] = {
    [NFCP_CLS_CAP_GET_INFO] = {
        .handler = nfcp_cls_cap_op_get_info,
        .handle_call = true
    },
    [NFCP_CLS_CAP_GET_RS] = {
        .handler = nfcp_cls_cap_op_get_resource,
        .handle_call = true
    },
    [NFCP_CLS_CAP_GET_PP] = {
        .handler = nfcp_cls_cap_op_get_peripheral,
        .handle_call = true
    },
    [NFCP_CLS_CAP_GET_MD] = {
        .handler = nfcp_cls_cap_op_get_module,
        .handle_call = true
    }
};

const nfcp_cls_t nfcp_cls_cap = {
    .cls_id = NFCP_CLS_CAP,
    .init = nfcp_cls_cap_init,
    .reset = nfcp_cls_cap_reset,
    .num_ops = NFCP_CLS_CAP_NUM_OPS,
    .ops = nfcp_cls_cap_ops
};

static void *nfcp_cls_cap_init(
    nfcp_t *nfcp)
{
    return NULL;
}

static void nfcp_cls_cap_reset(
    nfcp_t *nfcp,
    void *cls_storage)
{
}
static void abort_call(
    nfcp_t *nfcp,
    uint8_t op,
    uint8_t seq_nr)
{
    /* TODO: Better error handling */
    int resp_len = 0;
    if (0 == nfcp_tx_take(nfcp)) {
        resp_len +=
            nfcp_set_header(&nfcp->tx_buffer[resp_len], NFCP_CLS_CAP, op, true, true, seq_nr);
        nfcp_tx_packet(nfcp, nfcp->tx_buffer, resp_len);
        nfcp_tx_give(nfcp);
    }
}

static nfcp_op_status_t nfcp_cls_cap_op_get_info(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length)
{
    int resp_len = 0;
    int i;
    if (0 == nfcp_tx_take(nfcp)) {
        resp_len +=
            nfcp_set_header(&nfcp->tx_buffer[resp_len], NFCP_CLS_CAP, NFCP_CLS_CAP_GET_INFO, true, true, seq_nr);

        pack_from_u16(&nfcp->tx_buffer[resp_len], rs_get_count());
        resp_len += 2;

        pack_from_u16(&nfcp->tx_buffer[resp_len], pp_get_count());
        resp_len += 2;

        pack_from_u16(&nfcp->tx_buffer[resp_len], md_get_decl_count());
        resp_len += 2;

        pack_from_u16(&nfcp->tx_buffer[resp_len], SystemCoreClock / 1000000ul);
        resp_len += 2;

        for (i = 0; nf_target[i] != '\0'; i++) {
            nfcp->tx_buffer[resp_len++] = nf_target[i];
        }

        nfcp_tx_packet(nfcp, nfcp->tx_buffer, resp_len);
        nfcp_tx_give(nfcp);
    }
    return NFCP_OP_STATUS_SUCCESS;
}

static nfcp_op_status_t nfcp_cls_cap_op_get_resource(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length)
{
    if (length != 5) {
        abort_call(nfcp, NFCP_CLS_CAP_GET_RS, seq_nr);
        return NFCP_OP_STATUS_SUCCESS;
    }

    uint16_t obj_id = pack_to_u16(payload + 0);
    uint8_t field_type = payload[2];
    uint16_t field_idx = pack_to_u16(payload + 3);

    const rs_decl_t *rs = rs_get_by_id(obj_id);
    if (rs == NULL) {
        abort_call(nfcp, NFCP_CLS_CAP_GET_RS, seq_nr);
        return NFCP_OP_STATUS_SUCCESS;
    }

    if (field_type == 0 && field_idx == 0) {
        if (0 == nfcp_tx_take(nfcp)) {
            int resp_len = 0;
            int i;
            resp_len +=
                nfcp_set_header(&nfcp->tx_buffer[resp_len], NFCP_CLS_CAP, NFCP_CLS_CAP_GET_RS, true, true, seq_nr);

            /* Object header */
            pack_from_u16(&nfcp->tx_buffer[resp_len + 0], obj_id);
            nfcp->tx_buffer[resp_len + 2] = field_type;
            pack_from_u16(&nfcp->tx_buffer[resp_len + 3], field_idx);
            resp_len += 5;

            /* Resource info */
            pack_from_u16(&nfcp->tx_buffer[resp_len + 0], rs->type);
            pack_from_u16(&nfcp->tx_buffer[resp_len + 2], rs->count_avail);
            resp_len += 4;

            for (i = 0; rs->name[i] != '\0'; i++) {
                nfcp->tx_buffer[resp_len++] = rs->name[i];
            }

            nfcp_tx_packet(nfcp, nfcp->tx_buffer, resp_len);
            nfcp_tx_give(nfcp);
        }
    } else {
        abort_call(nfcp, NFCP_CLS_CAP_GET_RS, seq_nr);
    }
    return NFCP_OP_STATUS_SUCCESS;
}

static nfcp_op_status_t nfcp_cls_cap_op_get_peripheral(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length)
{
    if (length != 5) {
        abort_call(nfcp, NFCP_CLS_CAP_GET_PP, seq_nr);
        return NFCP_OP_STATUS_SUCCESS;
    }

    uint16_t obj_id = pack_to_u16(payload + 0);
    uint8_t field_type = payload[2];
    uint16_t field_idx = pack_to_u16(payload + 3);

    uint16_t num_arg_opts;
    int i;

    const pp_inst_decl_t *pp = pp_get_by_index(obj_id);
    if (pp == NULL) {
        abort_call(nfcp, NFCP_CLS_CAP_GET_PP, seq_nr);
        return NFCP_OP_STATUS_SUCCESS;
    }

    /* Count arg opts */
    num_arg_opts = 0;
    while (pp->rscs[num_arg_opts].tag != NULL) {
        num_arg_opts++;
    }

    if (field_type == 0 && field_idx == 0) {
        /* Peripheral info */
        if (0 == nfcp_tx_take(nfcp)) {
            int resp_len = 0;
            resp_len +=
                nfcp_set_header(&nfcp->tx_buffer[resp_len], NFCP_CLS_CAP, NFCP_CLS_CAP_GET_PP, true, true, seq_nr);

            /* Object header */
            pack_from_u16(&nfcp->tx_buffer[resp_len + 0], obj_id);
            nfcp->tx_buffer[resp_len + 2] = field_type;
            pack_from_u16(&nfcp->tx_buffer[resp_len + 3], field_idx);
            resp_len += 5;

            /* Resource info */
            nfcp->tx_buffer[resp_len + 0] = pp->decl->type;
            pack_from_u16(&nfcp->tx_buffer[resp_len + 1], pp->decl->num_rscs);
            pack_from_u16(&nfcp->tx_buffer[resp_len + 3], num_arg_opts);
            resp_len += 5;

            for (i = 0; pp->tag[i] != '\0'; i++) {
                nfcp->tx_buffer[resp_len++] = pp->tag[i];
            }

            nfcp_tx_packet(nfcp, nfcp->tx_buffer, resp_len);
            nfcp_tx_give(nfcp);
        }
    } else if (field_type == 1 && field_idx < num_arg_opts) {
        /* Argument info */
        if (0 == nfcp_tx_take(nfcp)) {
            int resp_len = 0;
            resp_len +=
                nfcp_set_header(&nfcp->tx_buffer[resp_len], NFCP_CLS_CAP, NFCP_CLS_CAP_GET_PP, true, true, seq_nr);

            /* Object header */
            pack_from_u16(&nfcp->tx_buffer[resp_len + 0], obj_id);
            nfcp->tx_buffer[resp_len + 2] = field_type;
            pack_from_u16(&nfcp->tx_buffer[resp_len + 3], field_idx);
            resp_len += 5;

            /* Resource info */
            pack_from_u16(&nfcp->tx_buffer[resp_len + 0], pp->rscs[field_idx].arg_nr);
            resp_len += 2;

            for (i = 0; pp->rscs[field_idx].tag[i] != '\0'; i++) {
                nfcp->tx_buffer[resp_len++] = pp->rscs[field_idx].tag[i];
            }

            nfcp_tx_packet(nfcp, nfcp->tx_buffer, resp_len);
            nfcp_tx_give(nfcp);
        }
    } else {
        abort_call(nfcp, NFCP_CLS_CAP_GET_RS, seq_nr);
    }
    return NFCP_OP_STATUS_SUCCESS;
}

static nfcp_op_status_t nfcp_cls_cap_op_get_module(
    nfcp_t *nfcp,
    void *cls_storage,
    bool is_call,
    uint8_t seq_nr,
    uint8_t *payload,
    int length)
{
    if (length != 5) {
        abort_call(nfcp, NFCP_CLS_CAP_GET_PP, seq_nr);
        return NFCP_OP_STATUS_SUCCESS;
    }

    uint16_t obj_id = pack_to_u16(payload + 0);
    uint8_t field_type = payload[2];
    uint16_t field_idx = pack_to_u16(payload + 3);

    uint16_t num_args;
    int i;

    const md_decl_t *md = md_get_decl_by_id(obj_id);
    if (md == NULL) {
        abort_call(nfcp, NFCP_CLS_CAP_GET_PP, seq_nr);
        return NFCP_OP_STATUS_SUCCESS;
    }

    /* Count args. For now, it's a string, but should probably be an object array later, so don't use string functions
     * */
    num_args = 0;
    while (md->args[num_args] != '\0') {
        num_args++;
    }

    if (field_type == 0 && field_idx == 0) {
        /* Module info */
        if (0 == nfcp_tx_take(nfcp)) {
            int resp_len = 0;
            resp_len +=
                nfcp_set_header(&nfcp->tx_buffer[resp_len], NFCP_CLS_CAP, NFCP_CLS_CAP_GET_MD, true, true, seq_nr);

            /* Object header */
            pack_from_u16(&nfcp->tx_buffer[resp_len + 0], obj_id);
            nfcp->tx_buffer[resp_len + 2] = field_type;
            pack_from_u16(&nfcp->tx_buffer[resp_len + 3], field_idx);
            resp_len += 5;

            /* Module info */
            pack_from_u16(&nfcp->tx_buffer[resp_len + 0], num_args);
            resp_len += 2;

            for (i = 0; md->name[i] != '\0'; i++) {
                nfcp->tx_buffer[resp_len++] = md->name[i];
            }

            nfcp_tx_packet(nfcp, nfcp->tx_buffer, resp_len);
            nfcp_tx_give(nfcp);
        }
    } else if (field_type == 1 && field_idx < num_args) {
        /* Argument info */
        if (0 == nfcp_tx_take(nfcp)) {
            int resp_len = 0;
            resp_len +=
                nfcp_set_header(&nfcp->tx_buffer[resp_len], NFCP_CLS_CAP, NFCP_CLS_CAP_GET_MD, true, true, seq_nr);

            /* Object header */
            pack_from_u16(&nfcp->tx_buffer[resp_len + 0], obj_id);
            nfcp->tx_buffer[resp_len + 2] = field_type;
            pack_from_u16(&nfcp->tx_buffer[resp_len + 3], field_idx);
            resp_len += 5;

            /* Argument */
            nfcp->tx_buffer[resp_len + 0] = md->args[field_idx];
            resp_len += 1;

            nfcp_tx_packet(nfcp, nfcp->tx_buffer, resp_len);
            nfcp_tx_give(nfcp);
        }
    } else {
        abort_call(nfcp, NFCP_CLS_CAP_GET_RS, seq_nr);
    }
    return NFCP_OP_STATUS_SUCCESS;
}
