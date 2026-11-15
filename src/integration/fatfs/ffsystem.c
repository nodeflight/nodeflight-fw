/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) Max Sikström
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
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define DEBUG 0

#if DEBUG
#include "core/log.h"
#define PRINTLN(...) log_println(__VA_ARGS__)
#else
#define PRINTLN(...) do {} while(0)
#endif

/*
 * One mutex per volume, and the system mutex at index FF_VOLUMES, which FatFs
 * only uses with FF_FS_LOCK. The heap can not free, so a mutex is created the
 * first time its volume is mounted and kept for the next mount.
 */
static SemaphoreHandle_t fs_mutexes[FF_VOLUMES + 1];

int ff_mutex_create (
    int vol)
{
    if (fs_mutexes[vol] == NULL) {
        fs_mutexes[vol] = xSemaphoreCreateMutex();
    }

    PRINTLN("ff_mutex_create(%d) = %p", vol, (void *) fs_mutexes[vol]);
    return (int) (fs_mutexes[vol] != NULL);
}

void ff_mutex_delete (
    int vol)
{
    /* Can't free, kept for the next mount of the volume */
    (void) vol;
    PRINTLN("ff_mutex_delete(%d)", vol);
}

int ff_mutex_take (
    int vol)
{
    PRINTLN("ff_mutex_take(%d)", vol);
    return (int) (xSemaphoreTake(fs_mutexes[vol], FF_FS_TIMEOUT) == pdTRUE);
}

void ff_mutex_give (
    int vol)
{
    PRINTLN("ff_mutex_give(%d)", vol);
    xSemaphoreGive(fs_mutexes[vol]);
}
