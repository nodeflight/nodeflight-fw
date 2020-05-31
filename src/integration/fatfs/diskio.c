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

#include "core/disk_access.h"

#define DEBUG 0

#if DEBUG
#include "vendor/tinyprintf/tinyprintf.h"
#define PRINTF(...) tfp_printf(__VA_ARGS__)
#else
#define PRINTF(...) do {} while(0)
#endif

DSTATUS disk_initialize (
    BYTE pdrv)
{
    PRINTF("disk_initialize(%u)\n", pdrv);
    disk_access_t *dacc = disk_access_get(pdrv);
    if (dacc != NULL) {
        return dacc->initialize(dacc->storage);
    }
    return STA_NOINIT;
}

DSTATUS disk_status (
    BYTE pdrv)
{
    PRINTF("disk_status(%u)\n", pdrv);
    disk_access_t *dacc = disk_access_get(pdrv);
    if (dacc != NULL) {
        return dacc->status(dacc->storage);
    }
    return STA_NOINIT;
}

DRESULT disk_read (
    BYTE pdrv,
    BYTE *buff,
    LBA_t sector,
    UINT count)
{
    PRINTF("disk_read(%u, %p, %lu, %u)\n", pdrv, buff, sector, count);
    disk_access_t *dacc = disk_access_get(pdrv);
    if (dacc != NULL) {
        return dacc->read(dacc->storage, buff, sector, count);
    }
    return RES_PARERR;
}

DRESULT disk_write (
    BYTE pdrv,
    const BYTE *buff,
    LBA_t sector,
    UINT count)
{
    PRINTF("disk_write(%u, %p, %lu, %u)\n", pdrv, buff, sector, count);
    disk_access_t *dacc = disk_access_get(pdrv);
    if (dacc != NULL) {
        return dacc->write(dacc->storage, buff, sector, count);
    }
    return RES_PARERR;
}

DRESULT disk_ioctl (
    BYTE pdrv,
    BYTE cmd,
    void *buff)
{
    PRINTF("disk_ioctl(%u, %u, %p)\n", pdrv, cmd, buff);
    disk_access_t *dacc = disk_access_get(pdrv);
    if (dacc != NULL) {
        return dacc->ioctl(dacc->storage, cmd, buff);
    }
    return RES_PARERR;
}
