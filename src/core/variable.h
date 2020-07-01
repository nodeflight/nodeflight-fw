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

typedef enum vr_type_s {
    VR_TYPE_NULL = 0,
    VR_TYPE_FLOAT,
    VR_TYPE_BOOL,
    VR_TYPE_INT
} vr_type_t;

/**
 * Initialize the subsystem for variable handling
 */
void vr_init(
    void);

/**
 * Connect all unconnected vairables
 *
 * @return 0 on success, negative on error
 */
int vr_connect(
    void);

/**
 * Register variables for a name
 *
 * The name should be the same as for the module providing them.
 *
 * Each letter in the type string refers to a pointer afterwards
 *
 * @param name Name of the variable. First word used, data copied to the module
 * @param types array of types, terminated by VR_TYPE_NULL
 */
int vr_register(
    const char *name,
    const vr_type_t *types,
    ...);

/**
 * Request a variable
 *
 * @param name name of the variable, in the format module.N. Note: will be overwritten
 * @param type variable type
 * @param target where to store the pointer to the variable
 * @return zero on success, negative or error
 */
int vr_request(
    char *name,
    vr_type_t type,
    void **target);
