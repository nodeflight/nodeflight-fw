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

#include "lib/crc.h"

uint16_t crc16(
    const uint8_t *buf,
    int len,
    uint16_t iv,
    uint16_t poly)
{
    uint16_t crc = iv;
    int i;
    int bit;
    for (i = 0; i < len; i++) {
        crc ^= ((uint16_t) buf[i] << 8);
        for (bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc <<= 1;
                crc ^= poly;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}
