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

#include "ff.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdbool.h>

#define DEBUG 0

#if DEBUG
#include "core/log.h"
#define PRINTLN(...) log_println(__VA_ARGS__)
#else
#define PRINTLN(...) do {} while(0)
#endif

static SemaphoreHandle_t fs_mutexes[FF_VOLUMES];
static bool fs_mutex_used[FF_VOLUMES];
static int fs_mutexes_allocated = 0;

int ff_cre_syncobj (
    BYTE vol,
    FF_SYNC_t *sobj)
{
    /* Reuse perviously allocated sempahore if possible */
    int i;
    SemaphoreHandle_t selected = NULL;

    for (i = 0; i < fs_mutexes_allocated && selected == NULL; i++) {
        if (!fs_mutex_used[i]) {
            fs_mutex_used[i] = true;
            selected = fs_mutexes[i];
        }
    }

    if (selected == NULL && fs_mutexes_allocated < FF_VOLUMES) {
        fs_mutexes[fs_mutexes_allocated] = xSemaphoreCreateMutex();
        if (fs_mutexes[fs_mutexes_allocated] != NULL) {
            fs_mutex_used[fs_mutexes_allocated] = true;
            selected = fs_mutexes[fs_mutexes_allocated];
            fs_mutexes_allocated++;
        }
    }

    *sobj = selected;

    PRINTLN("ff_cre_syncobj() = %p", (void *) selected);
    return (int) (selected != NULL);
}

int ff_del_syncobj (
    FF_SYNC_t sobj)
{
    int i;
    PRINTLN("ff_del_syncobj(%p)", (void *) sobj);

    /* Can't free, mark as unused and available for reuse */
    for (i = 0; i < fs_mutexes_allocated; i++) {
        if (fs_mutexes[i] == sobj) {
            fs_mutex_used[i] = false;
            return 1;
        }
    }
    return 0;
}

int ff_req_grant (
    FF_SYNC_t sobj)
{
    PRINTLN("ff_req_grant(%p)", (void *) sobj);
    return (int) (xSemaphoreTake(sobj, FF_FS_TIMEOUT) == pdTRUE);
}

void ff_rel_grant (
    FF_SYNC_t sobj)
{
    PRINTLN("ff_rel_grant(%p)", (void *) sobj);
    xSemaphoreGive(sobj);
}
