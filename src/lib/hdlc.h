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

#define HDLC_FRAME_BOUNDARY 0x7e
#define HDLC_ESCAPE_CHAR    0x7d
#define HDLC_ESCAPE_BITS    0x20

/* Escape for transmission, in case a link needs them. Not used otherwise */
#define HDLC_XON            0x17
#define HDLC_XOFF           0x19

/**
 * Unstuff packet in-place
 *
 * Packet may end with frame boundary, which is ignored. It must not contain any frame boundaies otherwise.
 *
 * @param buf byte-stuffed payload, which will be updated.
 * @param len unstuffed packet length
 * @return output length if successful, negative on error or abort sequence
 */
int hdlc_frame_unstuff(
    uint8_t *buf,
    int len);

/**
 * Stuff packet in-place
 *
 * The packet will increase in size to at most double + 1 byte. Frame boundary will always be added at the end
 *
 * @param buf raw payload, which will be updated.
 * @param len unstuffed packet length
 * @return output length if successful, negative on error or abort sequence
 */
int hdlc_frame_stuff(
    uint8_t *buf,
    int len);
