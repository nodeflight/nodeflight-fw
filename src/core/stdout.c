#include "core/stdout.h"
#include "resource/resource.h"

#include <stddef.h>

const resource_instance_decl_t *stdout_resource;

int stdout_init(
    const char *resource_tag)
{
    stdout_resource = resource_get_by_tag("uart3_d8_d9");
    if(stdout_resource == NULL) {
        return -1;
    }
    return 0;
}