#pragma once

#include <stdint.h>

/**********************
 * Types
 **********************/
typedef enum peripheral_type_e {
    PERIPHERAL_NONE = 0,
    PERIPHERAL_SERIAL,
    NUM_PERIPHERAL_TYPE
} peripheral_type_t;

/**********************
 * Common
 **********************/
typedef struct peripheral_decl_s peripheral_decl_t;
typedef struct peripheral_instance_decl_s peripheral_instance_decl_t;
typedef struct peripheral_instance_resource_s peripheral_instance_resource_t;

struct peripheral_decl_s {
    peripheral_type_t type;
    const char *name;
    void *callbacks;
    uint16_t num_resources;
    uint16_t storage_size;
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

#define PERIPHERAL_TYPE_DECL(_name, _type, _num_resources, _storage_type, _callbacks) \
    const peripheral_decl_t peripheral_ ## _name ## _decl = { \
        .type = _type, \
        .name = #_name, \
        .callbacks = (void *) &_callbacks, \
        .num_resources = _num_resources, \
        .storage_size = sizeof(_storage_type) \
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

const char *peripheral_get_type_name(
    peripheral_type_t type);

const peripheral_instance_decl_t *peripheral_get_by_index(
    int index);

const int peripheral_get_count(
    void);

const peripheral_instance_decl_t *peripheral_get_by_tag(
    peripheral_type_t type,
    const char *tag);

const peripheral_instance_resource_t *peripheral_get_resource_by_tag(
    const peripheral_instance_decl_t *peripheral,
    int arg_nr,
    const char *tag);

/**********************
 * Type: serial
 **********************/

typedef struct peripheral_serial_s peripheral_serial_t;

struct peripheral_serial_s {
    void (*init)(
        peripheral_serial_t *instance,
        const peripheral_instance_decl_t *variant,
        const char *tag);
    void (*write)(
        peripheral_serial_t *instance,
        void *buf,
        int bytes);
};

peripheral_serial_t *peripheral_serial_init_by_tag(
    const char *tag);

static inline void peripheral_serial_write(
    peripheral_serial_t *instance,
    void *buf,
    int bytes)
{
    instance->write(instance, buf, bytes);
}