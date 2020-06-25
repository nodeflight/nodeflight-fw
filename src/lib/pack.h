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

#include <stdint.h>

static inline uint32_t pack_to_u32(
    const uint8_t *ptr)
{
    return (((uint32_t) ptr[0]) << 0)
           | (((uint32_t) ptr[1]) << 8)
           | (((uint32_t) ptr[2]) << 16)
           | (((uint32_t) ptr[3]) << 24);
}

static inline uint16_t pack_to_u16(
    const uint8_t *ptr)
{
    return (((uint16_t) ptr[0]) << 0)
           | (((uint16_t) ptr[1]) << 8);
}

static inline void pack_from_u32(
    uint8_t *ptr,
    uint32_t val)
{
    ptr[0] = (val >> 0) & 0xff;
    ptr[1] = (val >> 8) & 0xff;
    ptr[2] = (val >> 16) & 0xff;
    ptr[3] = (val >> 24) & 0xff;
}

static inline void pack_from_u16(
    uint8_t *ptr,
    uint16_t val)
{
    ptr[0] = (val >> 0) & 0xff;
    ptr[1] = (val >> 8) & 0xff;
}
