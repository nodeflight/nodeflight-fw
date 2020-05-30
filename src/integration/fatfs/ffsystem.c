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
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

int ff_cre_syncobj (
    BYTE vol,
    FF_SYNC_t *sobj)
{
    *sobj = xSemaphoreCreateMutex();
    return (int) (*sobj != NULL);
}

int ff_del_syncobj (
    FF_SYNC_t sobj)
{
    vSemaphoreDelete(sobj);
    return 1;
}

int ff_req_grant (
    FF_SYNC_t sobj)
{
    return (int) (xSemaphoreTake(sobj, FF_FS_TIMEOUT) == pdTRUE);
}

void ff_rel_grant (
    FF_SYNC_t sobj)
{
    xSemaphoreGive(sobj);
}
