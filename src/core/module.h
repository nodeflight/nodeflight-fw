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

typedef union md_arg_u md_arg_t;
typedef struct md_arg_decl_s md_arg_decl_t;
typedef struct md_decl_s md_decl_t;

typedef enum md_arg_mode_s {
    MD_ARG_MODE_NORMAL,
    MD_ARG_MODE_OPTIONAL
} md_arg_mode_t;

typedef enum md_arg_type_s {
    MD_ARG_TYPE_PERIPHERAL,
    MD_ARG_TYPE_SCHEDULER,
    MD_ARG_TYPE_LINK,
    MD_ARG_TYPE_STRING,
    MD_ARG_TYPE_CONST
} md_arg_type_t;

#include "core/interface.h"
#include "core/scheduler.h"
#include "core/variable.h"

union md_arg_u {
    if_header_t *iface;
    sc_t *sched;
    char *name;
    int32_t const_int;
    float const_float;
};

struct md_arg_decl_s {
    const char *name;
    md_arg_mode_t mode;
    md_arg_type_t type;
    uint8_t subtype; /** Depends on type */
};

struct md_decl_s {
    const char *name;
    const md_arg_decl_t *args;
    const vr_named_type_t *outputs;
    int (*init)(
        const md_decl_t *md,
        const char *name,
        md_arg_t *args);
};

#define _MD_SECTION(_name) __attribute__ ((section(".nf_module." #_name), used))

/**
 * Declare a module
 *
 * _args is a string defining the types each arg to the module:
 *
 *  prefix with '?': Argument is optional. In config, put a '-'
 *
 * - 'p' = peripheral
 * - 's' = scheduler
 * - 'n' - name (string)
 * - 'ci' = const int32
 * - 'cf' = const float
 *
 * @param _name name of the module, and section name. Unique, without quotes
 * @param _init init function for the module
 * @param ... list of arguments, as MD_ARG_DECL()
 */
#define MD_DECL(_name, _init, ...) \
    const md_decl_t md_ ## _name ## _decl _MD_SECTION(_name) = { \
        .name = #_name, \
        .init = _init, \
        __VA_ARGS__ \
    }

/**
 * Declare a list of arguments
 */
#define MD_DECL_ARGS(...) \
    .args = (const md_arg_decl_t[]) { \
        __VA_ARGS__, \
        {0} \
    }

/**
 * Declare a module argument, as part of MD_DECL
 */
#define MD_ARG_DECL(_name, _mode, _type, _subtype) { \
        .name = _name, \
        .mode = _mode, \
        .type = _type, \
        .subtype = _subtype \
}

/**
 * Declare a list of outputs
 */
#define MD_DECL_OUTPUTS(...) \
    .outputs = (const vr_named_type_t[]) { \
        __VA_ARGS__, \
        VR_NULL_TYPE \
    }

/**
 * Declare a module argument, as part of MD_DECL
 */
#define MD_OUTPUT_DECL(_name, _type) { \
        .name = _name, \
        .type = _type, \
}

int md_init(
    const char *mdname,
    const char *name,
    int argc,
    char **argv);

const md_decl_t *md_get_decl_by_id(
    int index);

int md_get_decl_count(
    void);
