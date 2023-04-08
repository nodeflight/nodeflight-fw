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

/**
 * Initialize random number generator
 *
 * Note that the backend needs to be initialized by the platform itself
 */
void random_init(
    void);

/**
 * Get a random number
 *
 * This method is thread safe, however, exeuction time may vary.
 *
 * For fast random numbers, consider using a pseudorandom library, for which this can be used as seed.
 *
 * @return 32 bit true random number
 */
uint32_t random_get(
    void);
