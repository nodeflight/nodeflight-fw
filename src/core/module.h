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
typedef struct md_decl_s md_decl_t;

#include "core/interface.h"
#include "core/scheduler.h"

union md_arg_u {
    if_header_t *iface;
    scheduler_t *sched;
};

struct md_decl_s {
    const char *name;
    const char *args;
    int (*init)(
        md_arg_t *args);
};

#define _MD_SECTION(_name) __attribute__ ((section(".nf_module." #_name), used))

/**
 * Declare a module
 *
 * _args is a string defining the types each arg to the module. One letter per argument:
 *
 * - 'p' = peripheral
 * - 's' = scheduler
 *
 * @param _name name of the module, and section name. Unique, without quotes
 * @param _args string of argument types, one char per type
 * @param _init init function for the module
 */
#define MD_DECL(_name, _args, _init) \
    const md_decl_t md_ ## _name ## _decl _MD_SECTION(_name) = { \
        .name = #_name, \
        .args = _args, \
        .init = _init \
    }

int md_init(
    const char *name,
    const char *config);
