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

typedef struct module_decl_s module_decl_t;

struct module_decl_s {
    const char *name;
    int (*init)(
        const char *config);
};

#define _MODULE_SECTION(_name) __attribute__ ((section(".nf_module." #_name), used))

#define MODULE_DECL(_name, _init) \
    const module_decl_t module_ ## _name ## _decl _MODULE_SECTION(_name) = { \
        .name = #_name, \
        .init = _init \
    }

int module_init(
    const char *name,
    const char *config);
