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

#pragma once

#include <stdint.h>

typedef struct disk_access_s disk_access_t;

#include "ff.h"
#include "diskio.h"

struct disk_access_s {
    const char *name;

    DSTATUS (*initialize)(
        void *storage);

    DSTATUS (*status)(
        void *storage);

    DRESULT (*read)(
        void *storage,
        BYTE *buff,
        LBA_t sector,
        UINT count);

    DRESULT (*write) (
        void *storage,
        const BYTE *buff,
        LBA_t sector,
        UINT count);

    DRESULT (*ioctl) (
        void *storage,
        BYTE cmd,
        void *buff);

    void *storage;
};

void disk_access_init(
    void);

int disk_access_register(
    disk_access_t *dacc);
