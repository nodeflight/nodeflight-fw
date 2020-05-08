#include "hwacc/peripheral.h"
#include "hwacc/resource.h"
#include "core/strops.h"

#include <string.h>
#include <stdlib.h>

const extern peripheral_instance_decl_t __nf_peripheral_start[];
const extern peripheral_instance_decl_t __nf_peripheral_end[];

const peripheral_instance_decl_t *peripheral_get_by_index(
    int index)
{
    return &__nf_peripheral_start[index];
}

const int peripheral_get_count(
    void)
{
    return __nf_peripheral_end - __nf_peripheral_start;
}

const peripheral_instance_decl_t *peripheral_get_by_tag(
    const char *tag)
{
    const peripheral_instance_decl_t *cur;
    for (cur = __nf_peripheral_start; cur < __nf_peripheral_end; cur++) {
        if (0 == strops_word_cmp(cur->tag, tag)) {
            return cur;
        }
    }
    return NULL;
}

const peripheral_instance_resource_t *peripheral_get_resource_by_tag(
    const peripheral_instance_decl_t *peripheral,
    int arg_nr,
    const char *tag)
{
    const peripheral_instance_resource_t *rsc;
    for (rsc = peripheral->resources; rsc->tag != NULL; rsc++) {
        if (rsc->arg_nr == arg_nr && 0 == strops_word_cmp(rsc->tag, tag)) {
            return rsc;
        }
    }
    return NULL;
}
