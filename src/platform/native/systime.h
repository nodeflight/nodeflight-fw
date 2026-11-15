/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) Max Sikström
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
 * System time of the native target
 *
 * The time of the host's monotonic clock, from the start of the program.
 */

/**
 * Start the system time
 *
 * The time is counted from this call.
 */
void systime_init(
    void);

/**
 * Get the time since systime_init in microseconds
 *
 * Wraps around after about 71 minutes. Also used as the run time counter of
 * FreeRTOS.
 */
uint32_t systime_get_runtime(
    void);
