#include "hwacc/resource.h"
#include "core/strops.h"

#include <stdbool.h>
#include <stddef.h>

#include <stdio.h>

const extern resource_decl_t __nf_resource_start[];
const extern resource_decl_t __nf_resource_end[];

const resource_decl_t *resource_get_by_id(
    int index)
{
    return &__nf_resource_start[index];
}

const int resource_get_count(
    void)
{
    return __nf_resource_end - __nf_resource_start;
}

const resource_decl_t *resource_get_by_tag(
    const char *tag)
{
    const resource_decl_t *rsc;
    for (rsc = __nf_resource_start; rsc < __nf_resource_end; rsc++) {
        if (0 == strops_word_cmp(rsc->name, tag)) {
            return rsc;
        }
    }
    return NULL;
}
