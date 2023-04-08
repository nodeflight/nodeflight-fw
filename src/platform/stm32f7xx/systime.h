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

#include <stdint.h>

/**
 * Provide a high resolution runtime counter
 *
 * To be able to measure timings with enough accuracy, a hardware timer is
 * required at at least microsecond resolution.
 *
 * provide a simple to access runtime counter that runs from system start, that
 * is always provided indepdendent of resource allocations.
 */

/**
 * Start the High resolution runtime counter
 */
void systime_init(
    void);

/**
 * Get current runtime in microseconds since start
 *
 * Note: wraps around after ~25 days of runtime
 */
uint32_t systime_get_runtime(
    void);
