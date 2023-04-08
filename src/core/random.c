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

#include "platform/platform.h"
#include "core/random.h"
#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t random_mutex;

void random_init(
    void)
{
    random_mutex = xSemaphoreCreateMutex();
}

uint32_t random_get(
    void)
{
    uint32_t random = 0;
    if (pdFALSE == xSemaphoreTake(random_mutex, portMAX_DELAY)) {
        /* Should never happen */
        return 0;
    }
    random = platform_random_get();
    xSemaphoreGive(random_mutex);
    return random;
}
