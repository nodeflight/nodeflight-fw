#pragma once

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