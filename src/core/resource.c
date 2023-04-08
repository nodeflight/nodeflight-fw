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

#include "core/resource.h"
#include "lib/strops.h"
#include "FreeRTOS.h"
#include "lib/varsection.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

VARSECTION_ACCESS(rs_decl_t, nf_resource)

/* Array, indexed according to position in __nf_resource_* */
static int rs_count;
static rs_state_t *rs_states = NULL;

static void rs_init_state(
    void)
{
    if (rs_states == NULL) {
        rs_count = __nf_resource_end - __nf_resource_start;
        int mem_size = sizeof(rs_state_t) * rs_count;
        rs_states = pvPortMalloc(mem_size);
        if (rs_states == NULL) {
            /* TODO: error handling */
            for (;;) {
            }
        }
        memset(rs_states, 0, mem_size);
    }
}

rs_state_t *rs_get_state(
    const rs_decl_t *rsc)
{
    rs_init_state();
    return &rs_states[rsc - __nf_resource_start];
}

const rs_decl_t *rs_get_by_id(
    int index)
{
    if (index < 0 || &__nf_resource_start[index] >= __nf_resource_end) {
        return NULL;
    }
    return &__nf_resource_start[index];
}

int rs_get_count(
    void)
{
    return __nf_resource_end - __nf_resource_start;
}

const rs_decl_t *rs_get_by_tag(
    const char *tag)
{
    const rs_decl_t *rsc;
    for (rsc = __nf_resource_start; rsc < __nf_resource_end; rsc++) {
        if (0 == strops_cmp(rsc->name, tag)) {
            return rsc;
        }
    }
    return NULL;
}

bool rs_allocate(
    const rs_decl_t *rsc)
{
    rs_state_t *state = rs_get_state(rsc);
    if (rsc->count_avail > state->count_allocated) {
        state->count_allocated++;
        return true;
    } else {
        return false;
    }
}
