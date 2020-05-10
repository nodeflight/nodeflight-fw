#include <stdio.h>

#include "core/stdout.h"
#include "platform/platform.h"

#include "hwacc/resource.h"
#include "hwacc/peripheral.h"

#include "FreeRTOS.h"
#include "task.h"

#include "system_stm32f7xx.h"

extern const char __l1conf_start[];

static void main_task(
    void *pvParameters);

int main(
    void)
{

    platform_init();

    if (stdout_init("uart3 pin_d08 pin_d09") < 0) {
        asm ("bkpt 255");
        return 1;
    }

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

    printf("\n\nStarting NodeFlight\n\n");

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
        printf("resource %3d: %-10s  type: %3u  avail: %3u/%-3u  ref: %08lx\n",
            i,
            rsc->name,
            rsc->type,
            state->count_allocated,
            rsc->count_avail,
            rsc->ref);
    }

    printf("layer1 config:\n");
    for (i = 0; __l1conf_start[i] != 0xff && __l1conf_start[i] != '\0'; i++) {
        printf("%c", __l1conf_start[i]);
    }

    printf("SystemCoreClock: %ld\n", SystemCoreClock);

    i = 0;
    next_wakeup_time = xTaskGetTickCount();
    for (;;) {
        /* Place this task in the blocked state until it is time to run again. */
        vTaskDelayUntil(&next_wakeup_time, 1000 / portTICK_PERIOD_MS);
        printf("Tick... %d\n", i++);
    }
}