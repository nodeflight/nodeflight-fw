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

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define RS_COUNT_INF 0xffff

typedef struct rs_decl_s rs_decl_t;
typedef struct rs_state_s rs_state_t;

struct rs_decl_s {
    char name[16];
    uint16_t type;
    uint16_t count_avail;
    uint32_t ref;
};

/* Running state of the rs */
struct rs_state_s {
    uint8_t count_allocated;
};

/* Add name to section so they can be sorted during linking */
#define _RS_SECTION(_name) VARSECTION_ATTR(nf_resource, _name)

#define RS_DECL(_name, _type, _count, _ref) \
    const rs_decl_t rs_ ## _name ## _decl _RS_SECTION(_name) = { \
        .name = #_name, \
        .type = _type, \
        .count_avail = _count, \
        .ref = _ref, \
    }

rs_state_t *rs_get_state(
    const rs_decl_t *rsc);

const rs_decl_t *rs_get_by_id(
    int index);

int rs_get_count(
    void);

const rs_decl_t *rs_get_by_tag(
    const char *tag);

bool rs_allocate(
    const rs_decl_t *rsc);
