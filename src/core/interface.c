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

#include "core/interface.h"
#include "lib/strops.h"
#include "FreeRTOS.h"

static if_rs_t *if_rs_allocate(
    const pp_inst_decl_t *peripheral,
    int argc,
    char *const *argv)
{
    int rsc_count = peripheral->decl->num_rscs;
    int i;

    if(rsc_count != argc) {
        return NULL;
    }

    if_rs_t *rscs = pvPortMalloc(sizeof(if_rs_t) * rsc_count);
    if (rscs == NULL) {
        return NULL;
    }

    /* Fetch resources */
    for (i = 0; i < rsc_count; i++) {
        if_rs_t *rsc = &rscs[i];
        rsc->decl = rs_get_by_tag(argv[i]);
        rsc->inst = pp_get_rs_by_tag(peripheral, i, argv[i]);
        if (rsc->decl == NULL || rsc->inst == NULL) {
            return NULL;
        }
    }

    /* Allocate resources */
    for (i = 0; i < rsc_count; i++) {
        if_rs_t *rsc = &rscs[i];
        if (!rs_allocate(rsc->decl)) {
            /* TODO: Properly handle if resources could not be allocated... */
            return NULL;
        }
    }

    return rscs;
}

if_header_t *if_create(
    int argc,
    char *const *argv,
    pp_type_t type)
{
    if (argc < 1) {
        return NULL;
    }
    const pp_inst_decl_t *decl = pp_get_by_tag(argv[0]);
    if (decl == NULL) {
        /* TODO: error handling */
        asm ("bkpt 255");
        return NULL;
    }
    if (type != PP_NONE && type != decl->decl->type) {
        /* Not the correct type, error */
        /* TODO: error handling */
        asm ("bkpt 255");
        return NULL;
    }

    if_header_t *iface = pvPortMalloc(decl->decl->storage_size);
    if (iface == NULL) {
        /* TODO: error handling */
        asm ("bkpt 255");
        return NULL;
    }
    iface->peripheral = decl;
    iface->rscs = if_rs_allocate(decl, argc - 1, &argv[1]);
    if (iface->rscs == NULL) {
        /* TODO: error handling */
        asm ("bkpt 255");
        return NULL;
    }
    if (decl->decl->init(iface) < 0) {
        /* TODO: error handling */
        asm ("bkpt 255");
        return NULL;
    }

    return iface;
}
