#include <stdio.h>

#include "core/stdout.h"
#include "platform/platform.h"

#include "hwacc/resource.h"
#include "hwacc/peripheral.h"

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
        printf("peripheral: %-10s  type: %-10s  tag: %-16s\n",
            inst->decl->name,
            peripheral_get_type_name(inst->decl->type),
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
        printf("resource %3d: %-10s  type: %3u  avail: %3u  ref: %08lx\n",
            i,
            rsc->name,
            rsc->type,
            rsc->count_avail,
            rsc->ref);
    }

    for (;;) {
    }
    return 0;
}
