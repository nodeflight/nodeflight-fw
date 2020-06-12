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
 * @param types Type string. Pointer will be stored, make sure not to use a temporary variable.
 */
int vr_register(
    const char *name,
    const char *types,
    ...);

/**
 * Request a variable
 *
 * @param name name of the variable, in the format module.N. Note: will be overwritten
 * @param type variable type, as character same as vr_register
 * @param target where to store the pointer to the variable
 * @return zero on success, negative or error
 */
int vr_request(
    char *name,
    char type,
    void **target);
