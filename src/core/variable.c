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

#include "core/variable.h"
#include "lib/map.h"
#include "lib/strops.h"

#include "FreeRTOS.h"

#include "core/log.h"

#include <stdarg.h>

typedef struct vr_source_s vr_source_t;
typedef struct vr_reloc_s vr_reloc_t;

struct vr_source_s {
    const char *fmt; /**< Format string for the source, or NULL if not registered */
    void **vars; /**< Pointers to variables */
    vr_reloc_t *reloc; /**< Pending relocations, if not bound yet */
};

struct vr_reloc_s {
    vr_reloc_t *next;
    void **dst;
    uint16_t index;
    char type;
};

static map_t *vr_sources;

/**
 * Pool of unused/previously used relocations structs
 *
 * Since they may be marked as unused during config read, they can be re-used
 */
static vr_reloc_t *vr_free_reloc;

static vr_reloc_t *vr_reloc_alloc(
    void)
{
    vr_reloc_t *rlc = NULL;
    if (vr_free_reloc != NULL) {
        rlc = vr_free_reloc;
        vr_free_reloc = vr_free_reloc->next;
    }
    if (rlc == NULL) {
        rlc = pvPortMalloc(sizeof(vr_reloc_t));
    }
    if (rlc == NULL) {
        return NULL;
    }

    rlc->next = NULL;
    rlc->dst = NULL;
    rlc->index = 0;
    rlc->type = 0;
    return rlc;
}

static void vr_reloc_free(
    vr_reloc_t *rlc)
{
    rlc->next = vr_free_reloc;
    vr_free_reloc = rlc;
}

static vr_source_t *vr_get_source(
    const char *name)
{
    vr_source_t *src;

    src = map_get(vr_sources, name);
    if (src == NULL) {
        src = pvPortMalloc(sizeof(vr_source_t));
        if (src == NULL) {
            return NULL;
        }
        src->fmt = NULL;
        src->vars = NULL;
        src->reloc = NULL;
        map_set(vr_sources, name, src);
    }

    return src;
}

static int vr_reloc_exec(
    vr_source_t *src,
    int index,
    char type,
    void **dst)
{
    /* Source is defined, perform relocation */
    int i;

    /* Is type valid? */
    for (i = 0; i < index; i++) {
        if (src->fmt[i] == '\0') {
            /* Index out of bounds */
            return -1;
        }
    }
    if (src->fmt[index] != type) {
        /* Invalid type */
        return -1;
    }

    /* Update relocation */
    *dst = src->vars[index];

    return 0;
}

void vr_init(
    void)
{
    vr_sources = map_create();
}

int vr_connect(
    void)
{
    map_iter_t iter;
    const char *name;
    vr_source_t *src;

    map_iter_start(&iter, vr_sources);
    while (map_iter_next(&iter, &name, (void **) &src)) {
        if (src->fmt == NULL) {
            /* Unregistered variables */
            return -1;
        }
        if (src->reloc != NULL) {
            /* Unresolved relocations, shouldn't happen if src->fmt is set */
            return -1;
        }
    }
    (void) vr_reloc_free;
    return 0;
}

int vr_register(
    const char *name,
    const char *types,
    ...)
{
    vr_source_t *src = vr_get_source(name);
    int num_args;
    int i;

    if (src->fmt != NULL) {
        /* Already registered */
        /* TODO: Error handling */
        return -1;
    }

    num_args = 0;
    while (types[num_args] != '\0') {
        num_args++;
    }

    src->fmt = types;
    src->vars = pvPortMalloc(sizeof(void *) * num_args);
    if (src->vars == NULL) {
        return -1;
    }

    va_list ap;
    va_start(ap, types);
    for (i = 0; i < num_args; i++) {
        src->vars[i] = va_arg(ap, void *);
    }
    va_end(ap);

    /* Update pending relocations */
    while (src->reloc != NULL) {
        vr_reloc_t *rlc = src->reloc;
        src->reloc = src->reloc->next;

        if (vr_reloc_exec(src, rlc->index, rlc->type, rlc->dst) < 0) {
            /* TODO: Error handling */
            return -1;
        }
        vr_reloc_free(rlc);
    }
    return 0;
}

int vr_request(
    char *name,
    char type,
    void **target)
{
    /* Split name and index */
    char *dot = NULL;
    int i;
    for (i = 0; name[i] != '\0'; i++) {
        if (name[i] == '.') {
            dot = &name[i];
        }
    }
    if (dot == NULL) {
        /* TODO: Error handling */
        return -1;
    }
    *dot = '\0';
    int32_t index = strops_word_to_int(dot + 1);

    /* Fetch source */
    vr_source_t *src = vr_get_source(name);

    if (src->fmt == NULL) {
        /* Source not yet registered, create relocation request */
        vr_reloc_t *rlc = vr_reloc_alloc();
        rlc->dst = target;
        rlc->type = type;
        rlc->index = index;

        rlc->next = src->reloc;
        src->reloc = rlc;
    } else {
        /* Source is registered, perform relocation */
        if (vr_reloc_exec(src, index, type, target) < 0) {
            /* TODO: Error handling */
            return -1;
        }
    }

    return 0;
}
