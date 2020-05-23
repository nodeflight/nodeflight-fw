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

#include "core/resource.h"
#include "core/peripheral.h"
#include "core/scheduler.h"
#include "core/variable.h"

#include "integration/heap.h"

#include "FreeRTOS.h"
#include "task.h"

#include "vendor/tinyprintf/tinyprintf.h"

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

    xTaskCreate(main_task, "main", 1024, NULL, 0, NULL);

    vTaskStartScheduler();
    return 0;
}

static void main_task(
    void *storage)
{
    vr_init();
    sc_init();

    cf_init();

    if (0 != sc_init_clients()) {
        /* TODO: Error handling */
    }
    if (0 != vr_connect()) {
        /* TODO: Error handling */
    }

    print_boot_message();

    sc_enable();

    vTaskSuspend(NULL);
    for (;;) {
    }
}
