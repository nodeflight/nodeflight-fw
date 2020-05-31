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

#include "core/config.h"

#include "platform/platform.h"

#include "core/disk_int.h"
#include "core/resource.h"
#include "core/peripheral.h"
#include "core/scheduler.h"
#include "core/variable.h"

#include "integration/heap.h"

#include "FreeRTOS.h"
#include "task.h"

#include "vendor/tinyprintf/tinyprintf.h"

#include "ff.h"

#include <string.h>

FATFS fs_ext;

#if configUSE_TRACE_FACILITY
static const char *task_state_name[] = {
    [eRunning] = "running",
    [eReady] = "ready",
    [eBlocked] = "blocked",
    [eSuspended] = "suspended",
    [eDeleted] = "deleted",
    [eInvalid] = "invalid"
};
#endif

static void print_boot_message(
    void)
{
    uint32_t mem_size;
    uint32_t mem_used;
    tfp_printf("\nNodeFlight loaded\n\n");

    mem_size = heap_get_size();
    mem_used = heap_get_usage();
    tfp_printf("memory usage: %lu / %lu (%lu%%)\n", mem_used, mem_size, (100UL * mem_used) / mem_size);

    tfp_printf("\n");
}

static void dummy_putc(
    void *storage,
    char c)
{
    (void) storage;
    (void) c;
}

static void main_task(
    void *storage);

int main(
    void)
{
    /* tfp_printf() shoudn't crash if stdout isn't loaded */
    init_printf(NULL, dummy_putc);

    vPortInitialiseBlocks();
    platform_init();

    xTaskCreate(main_task, "main", 1024, NULL, 1, NULL);

    vTaskStartScheduler();
    return 0;
}

static void main_task(
    void *storage)
{

    vr_init();
    sc_init();

    disk_int_init();

    if (0 != cf_init("/int/boot.cfg")) {
        tfp_printf("Error: processing config\n");
        vTaskSuspend(NULL);
        for (;;) {
        }
    }

    if (0 != vr_connect()) {
        tfp_printf("Error: connecting variables\n");
        vTaskSuspend(NULL);
        for (;;) {
        }
    }

    if (0 != sc_init_clients()) {
        tfp_printf("Error: initializing scheduler clients\n");
        vTaskSuspend(NULL);
        for (;;) {
        }
    }

    print_boot_message();

    sc_enable();

#if configUSE_TRACE_FACILITY
    {
        TaskStatus_t tasks[32];
        UBaseType_t num_tasks;
        uint32_t runtime;
        UBaseType_t i;

        vTaskDelay(pdMS_TO_TICKS(1000));
        for (;;) {
            num_tasks = uxTaskGetSystemState(tasks, 32, &runtime);
            tfp_printf("\nnum_tasks = %lu  runtime = %lu\n", num_tasks, runtime);
            for (i = 0; i < num_tasks; i++) {
                TaskStatus_t *t = &tasks[i];
                tfp_printf("task[%2lu]: %-20s %9s prio=%2lu (%2lu), stack left=%4u\n",
                    t->xTaskNumber,
                    t->pcTaskName,
                    task_state_name[t->eCurrentState],
                    t->uxCurrentPriority,
                    t->uxBasePriority,
                    t->usStackHighWaterMark
                );
            }

            vTaskDelay(pdMS_TO_TICKS(10000));
        }
    }
#endif
    vTaskSuspend(NULL);
}
