#pragma once

typedef enum {
    RESOURCE_NONE = 0,
    RESOURCE_SERIAL,
    NUM_RESOURCE_TYPE
} resource_type_t;

typedef struct {
    resource_type_t type;
    const char *name;
} resource_decl_t;

typedef struct {
    const resource_decl_t *decl;
    const char *tag;
    const char *descr;
    const void *storage;
} resource_instance_decl_t;

#define RESOURCE_TYPE_DECL(_name, _type) \
    const static resource_decl_t resource_ ## _name ## _decl = { \
        .type = _type, \
        .name = #_name \
    }

/* Add name to section so they can be sorted during linking */
#define _RESOURCE_SECTION(_name, _tag) __attribute__ ((section(".nf_resource." #_name "_" #_tag), used))

#define RESOURCE_INSTANCE_DECL(_name, _tag, _descr, _storage) \
    const static resource_instance_decl_t resource_instance_ ##  _tag ## _decl _RESOURCE_SECTION(_name, _tag) = { \
        .decl = &resource_ ## _name ## _decl, \
        .tag = #_tag, \
        .descr = _descr, \
    }

const char *resource_get_type_name(
    resource_type_t type);

const resource_instance_decl_t *resource_get_by_index(
    int index);

const resource_instance_decl_t *resource_get_by_tag(
    const char *tag);

const int resource_get_count(
    void);