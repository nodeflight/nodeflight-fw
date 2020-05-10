#include <stdio.h>

#include "core/stdout.h"
#include "platform/platform.h"

#include "hwacc/resource.h"
#include "hwacc/peripheral.h"

extern const char __l1conf_start[];

int main(
    void)
{
    int i;
    int count;

    platform_init();

    if (stdout_init("uart3 pin_d08 pin_d09") < 0) {
        asm ("bkpt 255");
        return 1;
    }

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

    for (;;) {
    }
    return 0;
}
