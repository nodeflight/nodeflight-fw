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

#include "core/disk_int.h"
#include "core/disk_access.h"

#include "ff.h"
#include "diskio.h"

#include "FreeRTOS.h"

#include <string.h>

extern const uint8_t __l1conf_start[];
extern const uint8_t __l1conf_end[];

static DSTATUS disk_int_initialize(
    void *storage);

static DSTATUS disk_int_status(
    void *storage);

static DRESULT disk_int_read(
    void *storage,
    BYTE *buff,
    LBA_t sector,
    UINT count);

static DRESULT disk_int_write (
    void *storage,
    const BYTE *buff,
    LBA_t sector,
    UINT count);

static DRESULT disk_int_ioctl (
    void *storage,
    BYTE cmd,
    void *buff);

int disk_int_init(
    void)
{
    disk_access_t *dacc_int;
    dacc_int = pvPortMalloc(sizeof(disk_access_t));
    if (dacc_int == NULL) {
        return -1;
    }
    dacc_int->name = "int";
    dacc_int->initialize = disk_int_initialize;
    dacc_int->status = disk_int_status;
    dacc_int->read = disk_int_read;
    dacc_int->write = disk_int_write;
    dacc_int->ioctl = disk_int_ioctl;
    dacc_int->storage = NULL;
    if (0 != disk_access_register(dacc_int)) {
        return -1;
    }
    return 0;
}

DSTATUS disk_int_initialize(
    void *storage)
{
    return STA_PROTECT;
}

DSTATUS disk_int_status(
    void *storage)
{
    return STA_PROTECT;
}

DRESULT disk_int_read(
    void *storage,
    BYTE *buff,
    LBA_t sector,
    UINT count)
{
    if (__l1conf_start + 512 * sector + 512 * count > __l1conf_end) {
        return RES_ERROR;
    }
    memcpy(buff, __l1conf_start + 512 * sector, 512 * count);
    return RES_OK;
}

DRESULT disk_int_write (
    void *storage,
    const BYTE *buff,
    LBA_t sector,
    UINT count)
{
    return RES_WRPRT;
}

DRESULT disk_int_ioctl (
    void *storage,
    BYTE cmd,
    void *buff)
{
    return RES_PARERR;
}
