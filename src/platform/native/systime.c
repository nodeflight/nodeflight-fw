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

#include "systime.h"

#include <time.h>

static struct timespec systime_start;

void systime_init(
    void)
{
    clock_gettime(CLOCK_MONOTONIC, &systime_start);
}

uint32_t systime_get_runtime(
    void)
{
    struct timespec now;
    int64_t us;

    clock_gettime(CLOCK_MONOTONIC, &now);
    us = (int64_t) (now.tv_sec - systime_start.tv_sec) * 1000000
        + (now.tv_nsec - systime_start.tv_nsec) / 1000;

    return (uint32_t) us;
}
