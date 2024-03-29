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

#include "core/config.h"

#include "platform/platform.h"

#include "core/disk_int.h"
#include "core/resource.h"
#include "core/peripheral.h"
#include "core/scheduler.h"
#include "core/variable.h"
#include "core/random.h"
#include "core/log.h"

#include "integration/heap.h"

#include "FreeRTOS.h"
#include "task.h"

#include "core/log.h"

#include "ff.h"

#include "systime.h"
#include "core/task_trace.h"

#include <string.h>

FATFS fs_ext;

static void print_boot_message(
    void)
{
    uint32_t mem_size;
    uint32_t mem_used;
    log_println("");
    log_println("");
    log_println("NodeFlight loaded");

    mem_size = heap_get_size();
    mem_used = heap_get_usage();
    log_println("memory usage: %lu / %lu (%lu%%)", mem_used, mem_size, (100UL * mem_used) / mem_size);

    log_println("");
}

static void main_task(
    void *storage);

int main(
    void)
{
    vPortInitialiseBlocks();
    platform_init();
    systime_init();

    xTaskCreate(main_task, "main", 1024, NULL, 1, NULL);

    vTaskStartScheduler();
    return 0;
}

static void main_task(
    void *storage)
{
    log_init();

    random_init();

    vr_init();
    sc_init();

    disk_int_init();

#if configUSE_TRACE_FACILITY
    task_trace_init();
#endif

    if (0 != cf_init("/int/boot.cfg")) {
        log_println("Error: processing config");
        vTaskSuspend(NULL);
        for (;;) {
        }
    }

    if (0 != vr_connect()) {
        log_println("Error: connecting variables");
        vTaskSuspend(NULL);
        for (;;) {
        }
    }

    if (0 != sc_init_clients()) {
        log_println("Error: initializing scheduler clients");
        vTaskSuspend(NULL);
        for (;;) {
        }
    }

    print_boot_message();

    sc_enable();

#if configUSE_TRACE_FACILITY
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        for (;;) {
            task_trace_print();
            vTaskDelay(pdMS_TO_TICKS(10000));
        }
    }
#endif
    vTaskSuspend(NULL);
}
