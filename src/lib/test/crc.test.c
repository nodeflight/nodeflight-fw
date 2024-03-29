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

#include "unittest/testcase.h"
#include "lib/crc.h"

NF_TESTCASE(crc_valid) {
    ck_assert_int_eq(0x0d03, crc16((const uint8_t[]) {0x01, 0x02, 0x03, 0x04}, 4, 0, CRC16_POLY_CCITT));
    ck_assert_int_eq(0x0000, crc16(NULL, 0, 0, CRC16_POLY_CCITT));
}
