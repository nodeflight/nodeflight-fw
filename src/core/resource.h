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
#include <stdbool.h>

#define RESOURCE_COUNT_INF 0xffff

typedef struct resource_decl_s resource_decl_t;
typedef struct resource_state_s resource_state_t;

struct resource_decl_s {
    char name[16];
    uint16_t type;
    uint16_t count_avail;
    uint32_t ref;
};

/* Running state of the resource */
struct resource_state_s {
    uint8_t count_allocated;
};

/* Add name to section so they can be sorted during linking */
#define _RESOURCE_SECTION(_name) __attribute__ ((section(".nf_resource." #_name), used))

#define RESOURCE_DECL(_name, _type, _count, _ref) \
    const resource_decl_t resource_ ## _name ## _decl _RESOURCE_SECTION(_name) = { \
        .name = #_name, \
        .type = _type, \
        .count_avail = _count, \
        .ref = _ref, \
    }

resource_state_t *resource_get_state(
    const resource_decl_t *rsc);

const resource_decl_t *resource_get_by_id(
    int index);

const int resource_get_count(
    void);

const resource_decl_t *resource_get_by_tag(
    const char *tag);

bool resource_allocate(
    const resource_decl_t *rsc);