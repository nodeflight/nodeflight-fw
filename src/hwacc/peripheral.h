#pragma once

#include <stdint.h>

/**********************
 * Common
 **********************/
typedef struct peripheral_decl_s peripheral_decl_t;
typedef struct peripheral_instance_decl_s peripheral_instance_decl_t;
typedef struct peripheral_instance_resource_s peripheral_instance_resource_t;

#include "hwacc/interface.h"

struct peripheral_decl_s {
    const char *name;
    uint16_t num_rscs;
    interface_header_t *(*init)(
        const peripheral_instance_decl_t *decl,
        interface_resource_t *rscs,
        const char *config);
};

struct peripheral_instance_decl_s {
    const peripheral_decl_t *decl;
    const char *tag;
    const peripheral_instance_resource_t *resources;
    void *storage;
};

struct peripheral_instance_resource_s {
    const char *tag;
    uint16_t arg_nr;
    uint16_t attr;
};

#define PERIPHERAL_TYPE_EXTERN(_name) \
    extern const peripheral_decl_t peripheral_ ## _name ## _decl

#define PERIPHERAL_TYPE_DECL(_name, _num_rscs, _init) \
    const peripheral_decl_t peripheral_ ## _name ## _decl = { \
        .name = #_name, \
        .num_rscs = _num_rscs, \
        .init = _init \
    }

/* Add name to section so they can be sorted during linking */
#define _PERIPHERAL_SECTION(_name, _tag) __attribute__ ((section(".nf_peripheral." #_name "_" #_tag), used))

#define PERIPHERAL_INSTANCE_RESOURCE(_arg_nr, _tag, _attr) \
    { \
        .tag = #_tag, \
        .arg_nr = _arg_nr, \
        .attr = _attr \
    }

#define PERIPHERAL_INSTANCE_DECL(_name, _tag, _storage, ...) \
    const static peripheral_instance_decl_t peripheral_instance_ ##  _tag ## _decl _PERIPHERAL_SECTION(_name, _tag) = { \
        .decl = (const peripheral_decl_t *)&peripheral_ ## _name ## _decl, \
        .tag = #_tag, \
        .resources = (const peripheral_instance_resource_t[]) { \
            __VA_ARGS__, \
            {.tag = NULL, .arg_nr = 0, .attr = 0} \
        }, \
        .storage = (_storage) \
    }

const peripheral_instance_decl_t *peripheral_get_by_index(
    int index);

const int peripheral_get_count(
    void);

const peripheral_instance_decl_t *peripheral_get_by_tag(
    const char *tag);

const peripheral_instance_resource_t *peripheral_get_resource_by_tag(
    const peripheral_instance_decl_t *peripheral,
    int arg_nr,
    const char *tag);