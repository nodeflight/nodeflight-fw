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

typedef struct if_rs_s if_rs_t;
typedef struct if_header_s if_header_t;

#include "core/peripheral.h"
#include "core/resource.h"

struct if_rs_s {
    const rs_decl_t *decl;
    const pp_inst_rs_t *inst;
};

/**
 * Interface header
 *
 * Each inteface has its storage. All interfaces has in common that the first argument
 * is this header, which means all interface pointers can be type cast to the header to
 * determine its type.
 */
struct if_header_s {
    const pp_inst_decl_t *peripheral;
    if_rs_t *rscs; /* resources in use */
};

if_header_t *if_create(
    int argc,
    char *const *argv,
    pp_type_t type);
