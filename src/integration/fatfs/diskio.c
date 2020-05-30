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

#include "ff.h"
#include "diskio.h"

DSTATUS disk_status (
    BYTE pdrv)
{
    return STA_NOINIT;
}

DSTATUS disk_initialize (
    BYTE pdrv)
{
    return STA_NOINIT;
}

DRESULT disk_read (
    BYTE pdrv,
    BYTE *buff,
    LBA_t sector,
    UINT count)
{
    return RES_PARERR;
}

DRESULT disk_write (
    BYTE pdrv,
    const BYTE *buff,
    LBA_t sector,
    UINT count)
{
    return RES_PARERR;
}

DRESULT disk_ioctl (
    BYTE pdrv,
    BYTE cmd,
    void *buff)
{
    return RES_PARERR;
}
