#pragma once

#include <stdint.h>

typedef struct resource_decl_s resource_decl_t;

struct resource_decl_s {
    char name[8];
    uint16_t type;
    uint8_t count_avail;
    uint32_t ref;
    void *(*alloc)(
        int resource_id);
};

/* Add name to section so they can be sorted during linking */
#define _RESOURCE_SECTION(_name) __attribute__ ((section(".nf_resource." #_name), used))

#define RESOURCE_DECL(_name, _type, _count, _alloc, _ref) \
    const resource_decl_t resource_ ## _name ## _decl _RESOURCE_SECTION(_name) = { \
        .name = #_name, \
        .type = _type, \
        .count_avail = _count, \
        .ref = _ref, \
        .alloc = _alloc \
    }

const resource_decl_t *resource_get_by_id(
    int index);

const int resource_get_count(
    void);

const resource_decl_t *resource_get_by_name_r(
    const char **tag);