#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct resource_decl_s resource_decl_t;
typedef struct resource_state_s resource_state_t;

struct resource_decl_s {
    char name[8];
    uint16_t type;
    uint8_t count_avail;
    uint32_t ref;
};

/* Running state of the resource */
struct resource_state_s {
    uint8_t count_allocated;
};

/* Add name to section so they can be sorted during linking */
#define _RESOURCE_SECTION(_name) __attribute__ ((section(".nf_resource." #_name), used))

#define RESOURCE_DECL(_name, _type, _count, _ref) \
    const resource_decl_t resource_ ## _name ## _decl _RESOURCE_SECTION(_name) = { \
        .name = #_name, \
        .type = _type, \
        .count_avail = _count, \
        .ref = _ref, \
    }

resource_state_t *resource_get_state(
    const resource_decl_t *rsc);

const resource_decl_t *resource_get_by_id(
    int index);

const int resource_get_count(
    void);

const resource_decl_t *resource_get_by_tag(
    const char *tag);

bool resource_allocate(
    const resource_decl_t *rsc);