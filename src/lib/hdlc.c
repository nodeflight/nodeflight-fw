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

#include "lib/hdlc.h"

int hdlc_frame_unstuff(
    uint8_t *buf,
    int len)
{
    int rd = 0;
    int wr = 0;
    uint8_t c;

    /* Truncate trailing frame boundary */
    if (len > 0 && buf[len - 1] == HDLC_FRAME_BOUNDARY) {
        len--;
    }
    while (rd < len) {
        c = buf[rd++];
        if (c == HDLC_FRAME_BOUNDARY) {
            /* Not allowed in packet */
            return -1;
        } else if (c == HDLC_ESCAPE_CHAR) {
            if (rd == len) {
                /* Abort sequence */
                return -1;
            }
            /* Escaped char */
            c = buf[rd++];
            buf[wr++] = c ^ HDLC_ESCAPE_BITS;
        } else {
            /* Normal char */
            buf[wr++] = c;
        }
    }
    return wr;
}
