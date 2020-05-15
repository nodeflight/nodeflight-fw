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

#include "core/peripheral.h"
#include "core/resource.h"
#include "lib/strops.h"

#include <string.h>
#include <stdlib.h>

const extern peripheral_instance_decl_t __nf_peripheral_start[];
const extern peripheral_instance_decl_t __nf_peripheral_end[];

const peripheral_instance_decl_t *peripheral_get_by_index(
    int index)
{
    return &__nf_peripheral_start[index];
}

const int peripheral_get_count(
    void)
{
    return __nf_peripheral_end - __nf_peripheral_start;
}

const peripheral_instance_decl_t *peripheral_get_by_tag(
    const char *tag)
{
    const peripheral_instance_decl_t *cur;
    for (cur = __nf_peripheral_start; cur < __nf_peripheral_end; cur++) {
        if (0 == strops_word_cmp(cur->tag, tag)) {
            return cur;
        }
    }
    return NULL;
}

const peripheral_instance_resource_t *peripheral_get_resource_by_tag(
    const peripheral_instance_decl_t *peripheral,
    int arg_nr,
    const char *tag)
{
    const peripheral_instance_resource_t *rsc;
    for (rsc = peripheral->resources; rsc->tag != NULL; rsc++) {
        if (rsc->arg_nr == arg_nr && 0 == strops_word_cmp(rsc->tag, tag)) {
            return rsc;
        }
    }
    return NULL;
}
