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

#include "core/peripheral.h"
#include "core/resource.h"
#include "lib/strops.h"
#include "lib/varsection.h"

#include <stddef.h>

VARSECTION_ACCESS(pp_inst_decl_t, nf_peripheral)

const pp_inst_decl_t *pp_get_by_index(
    int index)
{
    if (index < 0 || &__nf_peripheral_start[index] >= __nf_peripheral_end) {
        return NULL;
    }
    return &__nf_peripheral_start[index];
}

const int pp_get_count(
    void)
{
    return __nf_peripheral_end - __nf_peripheral_start;
}

const pp_inst_decl_t *pp_get_by_tag(
    const char *tag)
{
    const pp_inst_decl_t *cur;
    for (cur = __nf_peripheral_start; cur < __nf_peripheral_end; cur++) {
        if (0 == strops_cmp(cur->tag, tag)) {
            return cur;
        }
    }
    return NULL;
}

const pp_inst_rs_t *pp_get_rs_by_tag(
    const pp_inst_decl_t *peripheral,
    int arg_nr,
    const char *tag)
{
    const pp_inst_rs_t *rsc;
    for (rsc = peripheral->rscs; rsc->tag != NULL; rsc++) {
        if (rsc->arg_nr == arg_nr && 0 == strops_cmp(rsc->tag, tag)) {
            return rsc;
        }
    }
    return NULL;
}
