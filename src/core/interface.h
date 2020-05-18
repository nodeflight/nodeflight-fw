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

typedef struct if_resource_s if_resource_t;
typedef struct if_header_s if_header_t;

#include "core/peripheral.h"
#include "core/resource.h"

struct if_resource_s {
    const resource_decl_t *decl;
    const peripheral_instance_resource_t *inst;
};

/**
 * Interface header
 *
 * Each inteface has its storage. All interfaces has in common that the first argument
 * is this header, which means all interface pointers can be type cast to the header to
 * determine its type.
 */
struct if_header_s {
    const peripheral_instance_decl_t *peripheral;
    if_resource_t *rscs; /* resources in use */
};

if_resource_t *if_resource_allocate(
    const peripheral_instance_decl_t *peripheral,
    const char **argp);

if_header_t *if_create(
    const char *config,
    peripheral_type_t type);
