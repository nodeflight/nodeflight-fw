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

/**********************
 * Common
 **********************/
typedef struct pp_decl_s pp_decl_t;
typedef struct pp_inst_decl_s pp_inst_decl_t;
typedef struct pp_inst_rs_s pp_inst_rs_t;

/**
 * Peripheral type
 *
 * The type models the method of access, not the external behaviour. If it modelled as a
 * serial port which handles stream-based IO, it is a serial port independent of being an UART
 * or TCP. Is is a value_out port independent of being a DSHOT, PWM or DAC output.
 */

typedef enum pp_type_s {
    PP_NONE = 0,
    PP_GPIO,
    PP_SERIAL,
    PP_PWM,
    PP_SPI
} pp_type_t;

#include "core/interface.h"

struct pp_decl_s {
    const char *name;
    pp_type_t type;
    uint16_t num_rscs;
    int (*init)(
        if_header_t *iface);
    int storage_size;
};

struct pp_inst_decl_s {
    const pp_decl_t *decl;
    const char *tag;
    const pp_inst_rs_t *rscs;
    void *storage;
};

struct pp_inst_rs_s {
    const char *tag;
    uint16_t arg_nr;
    uint16_t attr;
};

#define PP_TYPE_EXTERN(_name) \
    extern const pp_decl_t pp_ ## _name ## _decl

#define PP_TYPE_DECL(_name, _type, _num_rscs, _init, _storage_size) \
    const pp_decl_t pp_ ## _name ## _decl = { \
        .name = #_name, \
        .type = _type, \
        .num_rscs = _num_rscs, \
        .init = _init, \
        .storage_size = _storage_size \
    }

/* Add name to section so they can be sorted during linking */
#define _PP_SECTION(_name, _tag) __attribute__ ((section(".nf_peripheral." #_name "_" #_tag), used))

#define PP_INST_RS(_arg_nr, _tag, _attr) \
    { \
        .tag = #_tag, \
        .arg_nr = _arg_nr, \
        .attr = _attr \
    }

#define PP_INST_RS_TERMINATION \
    { \
        .tag = NULL, \
        .arg_nr = 0, \
        .attr = 0 \
    }

#define PP_INST_DECL(_name, _tag, _storage, ...) \
    const static pp_inst_decl_t pp_inst_ ##  _tag ## _decl _PP_SECTION(_name, _tag) = { \
        .decl = (const pp_decl_t *) &pp_ ## _name ## _decl, \
        .tag = #_tag, \
        .rscs = (const pp_inst_rs_t[]) { \
            __VA_ARGS__, \
            PP_INST_RS_TERMINATION \
        }, \
        .storage = (_storage) \
    }

const pp_inst_decl_t *pp_get_by_index(
    int index);

const int pp_get_count(
    void);

const pp_inst_decl_t *pp_get_by_tag(
    const char *tag);

const pp_inst_rs_t *pp_get_rs_by_tag(
    const pp_inst_decl_t *peripheral,
    int arg_nr,
    const char *tag);
