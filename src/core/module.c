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

#include "core/module.h"
#include "lib/strops.h"

const extern md_decl_t __nf_module_start[];
const extern md_decl_t __nf_module_end[];

int md_init(
    const char *name,
    const char *config)
{
    const md_decl_t *cur;
    for (cur = __nf_module_start; cur < __nf_module_end; cur++) {
        if (strops_word_cmp(cur->name, name) == 0) {
            return cur->init(config);
        }
    }
    return -1;
}
