#include "core/resource.h"
#include "lib/strops.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

const extern resource_decl_t __nf_resource_start[];
const extern resource_decl_t __nf_resource_end[];

/* Array, indexed according to position in __nf_resource_* */
static int resource_count;
static resource_state_t *resource_states = NULL;

static void resource_init_state(
    void)
{
    if (resource_states == NULL) {
        resource_count = __nf_resource_end - __nf_resource_start;
        int mem_size = sizeof(resource_state_t) * resource_count;
        resource_states = malloc(mem_size);
        if (resource_states == NULL) {
            /* TODO: error handling */
            asm ("bkpt 255");
            for (;;) {
            }
        }
        memset(resource_states, 0, mem_size);
    }
}

resource_state_t *resource_get_state(
    const resource_decl_t *rsc)
{
    resource_init_state();
    return &resource_states[rsc - __nf_resource_start];
}

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

bool resource_allocate(
    const resource_decl_t *rsc)
{
    resource_state_t *state = resource_get_state(rsc);
    if (rsc->count_avail > state->count_allocated) {
        state->count_allocated++;
        return true;
    } else {
        return false;
    }
}