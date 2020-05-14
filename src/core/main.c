#include <stdio.h>

#include "core/config.h"

#include "platform/platform.h"

#include "core/resource.h"
#include "core/peripheral.h"

#include "FreeRTOS.h"
#include "task.h"

#include "stm32.h"

static void main_task(
    void *pvParameters);

int main(
    void)
{

    platform_init();

    config_init();

    xTaskCreate(main_task,
        "main",
        1024,
        NULL,
        tskIDLE_PRIORITY,
        NULL);

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

    printf("Starting NodeFlight\n");

    printf("\nPeripherals:\n\n");

    count = peripheral_get_count();
    for (i = 0; i < count; i++) {
        const peripheral_instance_decl_t *inst = peripheral_get_by_index(i);
        printf("peripheral: %-10s  tag: %-16s\n",
            inst->decl->name,
            inst->tag);

        const peripheral_instance_resource_t *rsc;
        for (rsc = inst->resources; rsc->tag != NULL; rsc++) {
            printf("    tag: %8s  arg_nr: %u  attr: %u\n",
                rsc->tag,
                rsc->arg_nr,
                rsc->attr);
        }
    }

    printf("\nResources:\n\n");

    count = resource_get_count();
    for (i = 0; i < count; i++) {
        const resource_decl_t *rsc = resource_get_by_id(i);
        resource_state_t *state = resource_get_state(rsc);
        printf("resource %3d: %-10s  type: %3u  avail: %3u/%-5u ref: %08lx\n",
            i,
            rsc->name,
            rsc->type,
            state->count_allocated,
            rsc->count_avail,
            rsc->ref);
    }

    printf("SystemCoreClock: %ld\n", SystemCoreClock);

    /* To track uart transmission delay */
    LL_GPIO_Init(GPIOA, &(LL_GPIO_InitTypeDef) {
        .Pin = 1 << 5,
        .Mode = LL_GPIO_MODE_OUTPUT,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = 0
    });

    i = 0;
    next_wakeup_time = xTaskGetTickCount();
    for (;;) {
        /* Place this task in the blocked state until it is time to run again. */
        vTaskDelayUntil(&next_wakeup_time, 1000 / portTICK_PERIOD_MS);
        LL_GPIO_SetOutputPin(GPIOA, 1 << 5);
        printf("Tick... %d\n", i++);
        LL_GPIO_ResetOutputPin(GPIOA, 1 << 5);
    }
}