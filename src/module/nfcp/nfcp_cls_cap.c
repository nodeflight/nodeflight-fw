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

static uint8_t *prepare_response(
    nfcp_t *nfcp,
    const uint8_t *payload,
    int payload_len,
    uint8_t op,
    uint8_t seq_nr,
    uint16_t *obj_id,
    uint8_t *field_type,
    uint16_t *field_idx)
{
    if (payload_len < 5) {
        return NULL;
    }

    *obj_id = pack_to_u16(payload + 0);
    *field_type = payload[2];
    *field_idx = pack_to_u16(payload + 3);

    int resp_len = 0;
    if (0 == nfcp_tx_take(nfcp)) {
        /* Packet header */
        resp_len +=
            nfcp_set_header(&nfcp->tx_buffer[resp_len], NFCP_CLS_CAP, op, true, true, seq_nr);

        /* Object header */
        pack_from_u16(&nfcp->tx_buffer[resp_len + 0], *obj_id);
        nfcp->tx_buffer[resp_len + 2] = *field_type;
        pack_from_u16(&nfcp->tx_buffer[resp_len + 3], *field_idx);
        resp_len += 5;

        return &nfcp->tx_buffer[resp_len];
    } else {
        return NULL;
    }
}

static void send_response(
    nfcp_t *nfcp,
    uint8_t *end_ptr)
{
    int length = end_ptr - nfcp->tx_buffer;

    nfcp_tx_packet(nfcp, nfcp->tx_buffer, length);
    nfcp_tx_give(nfcp);
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
    uint16_t obj_id;
    uint8_t field_type;
    uint16_t field_idx;
    int i;

    uint8_t *ptr = prepare_response(
        nfcp,
        payload, length, NFCP_CLS_CAP_GET_RS, seq_nr, &obj_id, &field_type, &field_idx);
    if (ptr == NULL) {
        /* TODO: Better error handling */
        return NFCP_OP_STATUS_SUCCESS;
    }

    const rs_decl_t *rs = rs_get_by_id(obj_id);
    if (rs == NULL) {
        /* TODO: Error handling */
    } else if (field_type == 0 && field_idx == 0) {
        /* Resource info */
        pack_from_u16(ptr + 0, rs->type);
        pack_from_u16(ptr + 2, rs->count_avail);
        ptr += 4;

        for (i = 0; rs->name[i] != '\0'; i++) {
            *(ptr++) = rs->name[i];
        }
    }
    send_response(nfcp, ptr);
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
    uint16_t obj_id;
    uint8_t field_type;
    uint16_t field_idx;
    int i;

    uint8_t *ptr = prepare_response(
        nfcp,
        payload, length, NFCP_CLS_CAP_GET_PP, seq_nr, &obj_id, &field_type, &field_idx);
    if (ptr == NULL) {
        /* TODO: Better error handling */
        return NFCP_OP_STATUS_SUCCESS;
    }

    const pp_inst_decl_t *pp = pp_get_by_index(obj_id);
    if (pp == NULL) {
        send_response(nfcp, ptr);
        return NFCP_OP_STATUS_SUCCESS;
    }

    /* Count arg opts */
    uint16_t num_arg_opts = 0;
    while (pp->rscs[num_arg_opts].tag != NULL) {
        num_arg_opts++;
    }

    if (field_type == 0 && field_idx == 0) {
        *ptr = pp->decl->type;
        pack_from_u16(ptr + 1, pp->decl->num_rscs);
        pack_from_u16(ptr + 3, num_arg_opts);
        ptr += 5;

        for (i = 0; pp->tag[i] != '\0'; i++) {
            *(ptr++) = pp->tag[i];
        }
    } else if (field_type == 1 && field_idx < num_arg_opts) {
        pack_from_u16(ptr + 0, pp->rscs[field_idx].arg_nr);
        ptr += 2;

        for (i = 0; pp->rscs[field_idx].tag[i] != '\0'; i++) {
            *(ptr++) = pp->rscs[field_idx].tag[i];
        }
    }

    send_response(nfcp, ptr);
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
    uint16_t obj_id;
    uint8_t field_type;
    uint16_t field_idx;
    int i;

    uint8_t *ptr = prepare_response(
        nfcp,
        payload, length, NFCP_CLS_CAP_GET_MD, seq_nr, &obj_id, &field_type, &field_idx);
    if (ptr == NULL) {
        /* TODO: Better error handling */
        return NFCP_OP_STATUS_SUCCESS;
    }

    const md_decl_t *md = md_get_decl_by_id(obj_id);
    if (md == NULL) {
        send_response(nfcp, ptr);
        return NFCP_OP_STATUS_SUCCESS;
    }

    /* Count args */
    uint16_t num_args = 0;
    while (md->args[num_args].name != NULL) {
        num_args++;
    }

    /* Count outputs */
    uint16_t num_outputs = 0;
    while (md->outputs && md->outputs[num_outputs].type != VR_TYPE_NULL) {
        num_outputs++;
    }

    if (field_type == 0 && field_idx == 0) {
        /* Module info */
        pack_from_u16(ptr + 0, num_args);
        pack_from_u16(ptr + 2, num_outputs);
        ptr += 4;

        for (i = 0; md->name[i] != '\0'; i++) {
            *(ptr++) = md->name[i];
        }
    } else if (field_type == 1 && field_idx < num_args) {
        const md_arg_decl_t *arg = &md->args[field_idx];
        *(ptr++) = arg->mode;
        *(ptr++) = arg->type;
        *(ptr++) = arg->subtype;

        for (i = 0; arg->name[i] != '\0'; i++) {
            *(ptr++) = arg->name[i];
        }
    } else if (field_type == 2 && field_idx < num_outputs) {
        const vr_named_type_t *output = &md->outputs[field_idx];
        *(ptr++) = output->type;

        for (i = 0; output->name[i] != '\0'; i++) {
            *(ptr++) = output->name[i];
        }
    }
    send_response(nfcp, ptr);
    return NFCP_OP_STATUS_SUCCESS;
}
