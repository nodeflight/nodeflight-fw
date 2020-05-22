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

int main(
    void)
{
    vPortInitialiseBlocks();

    platform_init();

    scheduler_init();

    /* Temporary, explicitly triggered from fport */
    scheduler_define("temp_sched", 0.009f);

    config_init();

    scheduler_init_clients();

    print_boot_message();

    vTaskStartScheduler();

    for (;;) {
    }

    return 0;
}
