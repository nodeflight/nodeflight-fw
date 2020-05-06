#include "hwacc/interface.h"
#include "core/strops.h"
#include <stdlib.h>

interface_resource_t *interface_resource_allocate(
    const peripheral_instance_decl_t *peripheral,
    const char **argp)
{
    int rsc_count = peripheral->decl->num_resources;
    int i;
    interface_resource_t *rscs = malloc(sizeof(interface_resource_t) * rsc_count);
    if (rscs == NULL) {
        return NULL;
    }

    for (i = 0; i < rsc_count; i++) {
        const char *arg = strops_next_word(argp);
        interface_resource_t *rsc = &rscs[i];
        rsc->decl = resource_get_by_tag(arg);
        rsc->inst = peripheral_get_resource_by_tag(peripheral, i, arg);
        if (rsc->decl == NULL || rsc->inst == NULL) {
            return NULL;
        }
    }

    return rscs;
}