#include "hwacc/resource.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

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

const resource_decl_t *resource_get_by_name_r(
    const char **tag)
{
    int i;
    int tag_len;
    const char *cur_tag = *tag;

    if(*tag == NULL) {
        return NULL;
    }

    tag_len = 0;
    while ((*tag)[tag_len] != '\0' && (*tag)[tag_len] != ' ') {
        tag_len++;
    }

    *tag += tag_len;
    if (**tag == ' ') {
        (*tag)++;
    } else {
        *tag = NULL;
    }

    for (i = 0; i < resource_get_count(); i++) {
        const resource_decl_t *rsc = resource_get_by_id(i);
        if (0 == memcmp(rsc->name, cur_tag, tag_len) && rsc->name[tag_len] == '\0') {
            return rsc;
        }
    }
    return NULL;
}
