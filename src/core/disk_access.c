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

#include "disk_access.h"

#include "ff.h"

#define DEBUG 0

#if DEBUG
#include "core/log.h"
#define PRINTLN(...) log_println(__VA_ARGS__)
#else
#define PRINTLN(...) do {} while(0)
#endif

static disk_access_t *disk_access[FF_VOLUMES];
static int disks_mounted = 0;

/* Mount names */
const static char volume_str_not_mounted[] = "not_mounted";
const char *VolumeStr[FF_VOLUMES];

void disk_access_init(
    void)
{
    int i;
    disks_mounted = 0;
    for (i = 0; i < FF_VOLUMES; i++) {
        disk_access[i] = NULL;
        VolumeStr[i] = volume_str_not_mounted;
    }
}

int disk_access_register(
    disk_access_t *dacc)
{
    FATFS *ffs;
    char id_string[3] = "0:";
    if (disks_mounted >= FF_VOLUMES) {
        return -1;
    }
    int disk_id = disks_mounted++;
    disk_access[disk_id] = dacc;
    VolumeStr[disk_id] = dacc->name;

    /* Allocate FATFS */
    ffs = pvPortMalloc(sizeof(FATFS));
    if (ffs == NULL) {
        return -1;
    }

    /* Mount via disk id */
    id_string[0] = '0' + disk_id;
    if (FR_OK != f_mount(ffs, id_string, 0)) {
        return -1;
    }
    return 0;
}

/*
 * diskio interface for fatfs
 */

DSTATUS disk_initialize (
    BYTE pdrv)
{
    PRINTLN("disk_initialize(%u)", pdrv);
    return disk_access[pdrv]->initialize(disk_access[pdrv]->storage);
}

DSTATUS disk_status (
    BYTE pdrv)
{
    PRINTLN("disk_status(%u)", pdrv);
    return disk_access[pdrv]->status(disk_access[pdrv]->storage);
}

DRESULT disk_read (
    BYTE pdrv,
    BYTE *buff,
    LBA_t sector,
    UINT count)
{
    PRINTLN("disk_read(%u, %p, %lu, %u)", pdrv, buff, sector, count);
    return disk_access[pdrv]->read(disk_access[pdrv]->storage, buff, sector, count);
}

DRESULT disk_write (
    BYTE pdrv,
    const BYTE *buff,
    LBA_t sector,
    UINT count)
{
    PRINTLN("disk_write(%u, %p, %lu, %u)", pdrv, buff, sector, count);
    return disk_access[pdrv]->write(disk_access[pdrv]->storage, buff, sector, count);
}

DRESULT disk_ioctl (
    BYTE pdrv,
    BYTE cmd,
    void *buff)
{
    PRINTLN("disk_ioctl(%u, %u, %p)", pdrv, cmd, buff);
    return disk_access[pdrv]->ioctl(disk_access[pdrv]->storage, cmd, buff);
}
