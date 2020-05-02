#include "resource/resource.h"

#include <string.h>

const extern resource_instance_decl_t __nf_resource_start[];
const extern resource_instance_decl_t __nf_resource_end[];

const static char *type_names[NUM_RESOURCE_TYPE] = {
    [RESOURCE_NONE] = "NONE",
    [RESOURCE_SERIAL] = "SERIAL"
};

const char *resource_get_type_name(
    resource_type_t type)
{
    return type_names[type];
}

const resource_instance_decl_t *resource_get_by_index(
    int index)
{
    return &__nf_resource_start[index];
}

const resource_instance_decl_t *resource_get_by_tag(
    const char *tag)
{
    const resource_instance_decl_t *cur;
    for(cur = __nf_resource_start; cur < __nf_resource_end; cur++) {
        if(0 == strcmp(cur->tag, tag)) {
            return cur;
        }
    }
    return NULL;
}

const int resource_get_count(
    void)
{
    return __nf_resource_end - __nf_resource_start;
}