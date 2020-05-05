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

    if (stdout_init("uart3_d08_d09") < 0) {
        asm ("bkpt 255");
        return 1;
    }

    printf("\n\nStarting NodeFlight\n\n");

    count = peripheral_get_count();
    for (i = 0; i < count; i++) {
        const peripheral_instance_decl_t *inst = peripheral_get_by_index(i);
        printf("peripheral: %-10s  type: %-10s  tag: %-16s  resources: %s\n",
            inst->decl->name,
            peripheral_get_type_name(inst->decl->type),
            inst->tag,
            inst->resources);

        const char *cur_tag;

        cur_tag = inst->resources;
        while (cur_tag != NULL) {
            const resource_decl_t *rsc = resource_get_by_name_r(&cur_tag);
            if (rsc != NULL) {
                printf("    resource: %-10s  type: %3u  avail: %3u  ref: %08lx\n",
                    rsc->name,
                    rsc->type,
                    rsc->count_avail,
                    rsc->ref);
            } else {
                printf("    resource unknown\n");
            }
        }
    }

    // count = resource_get_count();
    // for (i = 0; i < count; i++) {
    // const resource_decl_t *rsc = resource_get_by_id(i);
    // printf("resource %3d: %-10s  type: %3u  avail: %3u  ref: %08lx\n",
    // i,
    // rsc->name,
    // rsc->type,
    // rsc->count_avail,
    // rsc->ref);
    // }

    for (;;) {
    }
    return 0;
}
