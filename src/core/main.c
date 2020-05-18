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

#include "FreeRTOS.h"
#include "task.h"

#include "stm32.h"

#include "vendor/tinyprintf/tinyprintf.h"

static void main_task(
    void *pvParameters);

int main(
    void)
{
    vPortInitialiseBlocks();

    platform_init();

    config_init();

    // xTaskCreate(main_task,
    // "main",
    // 1024,
    // NULL,
    // tskIDLE_PRIORITY,
    // NULL);
    (void) main_task;

    vTaskStartScheduler();
    asm ("bkpt 255");

    return 0;
}

void main_task(
    void *pvParameters)
{
    int i;
    int count;
    TickType_t next_wakeup_time;

    tfp_printf("Starting NodeFlight\n");

    tfp_printf("\nPeripherals:\n\n");

    count = pp_get_count();
    for (i = 0; i < count; i++) {
        const pp_instance_decl_t *inst = pp_get_by_index(i);
        tfp_printf("peripheral: %-10s  tag: %-16s\n",
            inst->decl->name,
            inst->tag);

        const pp_instance_rs_t *rsc;
        for (rsc = inst->rscs; rsc->tag != NULL; rsc++) {
            tfp_printf("    tag: %8s  arg_nr: %u  attr: %u\n",
                rsc->tag,
                rsc->arg_nr,
                rsc->attr);
        }
    }

    tfp_printf("\nResources:\n\n");

    count = rs_get_count();
    for (i = 0; i < count; i++) {
        const rs_decl_t *rsc = rs_get_by_id(i);
        rs_state_t *state = rs_get_state(rsc);
        tfp_printf("rs %3d: %-10s  type: %3u  avail: %3u/%-5u ref: %08lx\n",
            i,
            rsc->name,
            rsc->type,
            state->count_allocated,
            rsc->count_avail,
            rsc->ref);
    }

    tfp_printf("SystemCoreClock: %ld\n", SystemCoreClock);

    /* To track uart transmission delay */
    LL_GPIO_Init(GPIOA, &(LL_GPIO_InitTypeDef) {
        .Pin = 1 << 5,
        .Mode = LL_GPIO_MODE_OUTPUT,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = 0
    });

    tfp_printf("Mem free: %u\n", xPortGetFreeHeapSize());

    i = 0;
    next_wakeup_time = xTaskGetTickCount();
    for (;;) {
        /* Place this task in the blocked state until it is time to run again. */
        vTaskDelayUntil(&next_wakeup_time, 1000 / portTICK_PERIOD_MS);
        LL_GPIO_SetOutputPin(GPIOA, 1 << 5);
        tfp_printf("Tick... %d\n", i++);
        LL_GPIO_ResetOutputPin(GPIOA, 1 << 5);
    }
}
