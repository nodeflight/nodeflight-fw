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

#include <stdbool.h>

/**
 * Maps objects to tags
 *
 * Tags is identified as strings, without whitespace and line breaks.
 *
 * Elements can be added to a map, or replacing old values. Elements can't be removed.
 *
 * Uses memory allocation, and updating is therfore only suitable for initial configuration loading.
 */

typedef struct map_s map_t;

/**
 * Iterator to traverse the map
 */
typedef struct map_iter_s map_iter_t;

/* Not to be accessed directly */
struct map_iter_s {
    const map_t *map;
    int next_index;
};

/**
 * Create a new map
 *
 * @return storage for a new map, or NULL on error
 */
map_t *map_create(
    void);

/**
 * Set a value in a map
 *
 * The tag will be copied into the map.
 *
 * If the value exists in the map, the old value will be overwritten
 *
 * @param map Map object
 * @param tag Tag for the object. Duplicated into the map. Terminated by space, line break or null-char
 * @param new_value Pointer to an object to store in the map.
 * @return 0 on success, negative on error
 */
int map_set(
    map_t *map,
    const char *tag,
    void *new_value);

/**
 * Get a value from the map
 *
 * @param map Map object
 * @param tag tag to lookup. Terminated by space, line break or null-char
 * @return pointer to object, or NULL if not available
 */
void *map_get(
    const map_t *map,
    const char *tag);

/**
 * Start iteration
 *
 * @param iter pointer to iterator element to initialzie
 * @param map map to iterate over
 */
void map_iter_start(
    map_iter_t *iter,
    const map_t *map);

/**
 * Get next element
 *
 * First call will return the first elements
 *
 * @param iter Iterator object
 * @param name where to store the name pointer
 * @param value pointer to store the pointer to the element
 * @return true if element found, false if end of map
 */
bool map_iter_next(
    map_iter_t *iter,
    const char **name,
    void **value);
