#include <stdio.h>

#include "core/stdout.h"
#include "platform/platform.h"
#include "resource/resource.h"

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

    count = resource_get_count();
    for (i = 0; i < count; i++) {
        const resource_instance_decl_t *inst = resource_get_by_index(i);
        printf("resource: %-10s  type: %-10s  tag: %-16s  desc: %s\n",
            inst->decl->name,
            resource_get_type_name(inst->decl->type),
            inst->tag,
            inst->descr);
    }

    for (;;) {
    }
    return 0;
}
