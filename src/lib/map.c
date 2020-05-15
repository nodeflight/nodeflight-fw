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

#include "lib/map.h"
#include "lib/strops.h"
#include "FreeRTOS.h"
#include <stdint.h>
#include <stddef.h>

#define MAP_ELEMS_PER_CHUNK 16

typedef struct map_elem_s map_elem_t;

struct map_elem_s {
    const char *tag;
    void *ptr;
};

struct map_s {
    map_elem_t elems[MAP_ELEMS_PER_CHUNK];
    map_t *next;
};

map_t *map_create(
    void)
{
    map_t *map = pvPortMalloc(sizeof(map_t));
    int i;
    if (map == NULL) {
        return NULL;
    }
    for (i = 0; i < MAP_ELEMS_PER_CHUNK; i++) {
        map->elems[i].tag = NULL;
        map->elems[i].ptr = NULL;
    }
    map->next = NULL;
    return map;
}

int map_set(
    map_t *map,
    const char *tag,
    void *new_value)
{
    int i;
    while (map != NULL) {
        for (i = 0; i < MAP_ELEMS_PER_CHUNK; i++) {
            if (map->elems[i].tag == NULL) {
                /* New element */
                map->elems[i].tag = strops_word_dup(tag);
                map->elems[i].ptr = new_value;
                if (map->elems[i].tag == NULL) {
                    return -1;
                }
                return 0;
            } else if (0 == strops_word_cmp(map->elems[i].tag, tag)) {
                /* Replace old */
                map->elems[i].ptr = new_value;
                return 0;
            }
        }
        if (map->next == NULL) {
            /* At end of chain, need more space */
            map->next = map_create();
        }
        /* If allocation fails, map will be NULL, and function will exit with error. */
        map = map->next;
    }
    return -1;
}

void *map_get(
    const map_t *map,
    const char *tag)
{
    int i;
    while (map != NULL) {
        for (i = 0; i < MAP_ELEMS_PER_CHUNK; i++) {
            if (map->elems[i].tag == NULL) {
                /* End of list */
                return NULL;
            } else if (0 == strops_word_cmp(map->elems[i].tag, tag)) {
                /* Correct value */
                return map->elems[i].ptr;
            }
        }
        map = map->next;
    }
    return NULL;
}
