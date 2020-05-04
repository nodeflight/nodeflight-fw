#include <stdio.h>

#include "core/stdout.h"
#include "platform/platform.h"
#include "hwacc/peripheral.h"

int main(
    void)
{
    int i;
    int count;

    platform_init();

    if (stdout_init("uart3_d08_d09") < 0) {
        asm ("bkpt 255");
        return 1;
    }

    printf("\n\nStarting NodeFlight\n\n");

    count = peripheral_get_count();
    for (i = 0; i < count; i++) {
        const peripheral_instance_decl_t *inst = peripheral_get_by_index(i);
        printf("peripheral: %-10s  type: %-10s  tag: %-16s  desc: %s\n",
            inst->decl->name,
            peripheral_get_type_name(inst->decl->type),
            inst->tag,
            inst->descr);
    }

    for (;;) {
    }
    return 0;
}
