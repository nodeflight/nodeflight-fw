#pragma once

/**********************
 * Types
 **********************/
typedef enum resource_type_e {
    RESOURCE_NONE = 0,
    RESOURCE_SERIAL,
    NUM_RESOURCE_TYPE
} resource_type_t;

/**********************
 * Common
 **********************/
typedef struct resource_decl_s resource_decl_t;
typedef struct resource_instance_decl_s resource_instance_decl_t;

struct resource_decl_s {
    resource_type_t type;
    const char *name;
    void *callbacks;
    int storage_size;
};

struct resource_instance_decl_s {
    const resource_decl_t *decl;
    const char *tag;
    const char *descr;
    const void *storage;
};

#define RESOURCE_TYPE_DECL(_name, _type, _storage_type, _callbacks) \
    const static resource_decl_t resource_ ## _name ## _decl = { \
        .type = _type, \
        .name = #_name, \
        .callbacks = (void *) &_callbacks, \
        .storage_size = sizeof(_storage_type) \
    }

/* Add name to section so they can be sorted during linking */
#define _RESOURCE_SECTION(_name, _tag) __attribute__ ((section(".nf_resource." #_name "_" #_tag), used))

#define RESOURCE_INSTANCE_DECL(_name, _tag, _descr, _storage) \
    const static resource_instance_decl_t resource_instance_ ##  _tag ## _decl _RESOURCE_SECTION(_name, _tag) = { \
        .decl = (const resource_decl_t *)&resource_ ## _name ## _decl, \
        .tag = #_tag, \
        .descr = _descr, \
        .storage = (_storage) \
    }

const char *resource_get_type_name(
    resource_type_t type);

const resource_instance_decl_t *resource_get_by_index(
    int index);

const int resource_get_count(
    void);

const resource_instance_decl_t *resource_get_by_tag(
    resource_type_t type,
    const char *tag);

/**********************
 * Type: serial
 **********************/

typedef struct resource_serial_s resource_serial_t;

struct resource_serial_s {
    void (*init)(
        resource_serial_t *instance,
        const resource_instance_decl_t *variant);
    void (*write)(
        resource_serial_t *instance,
        void *buf,
        int bytes);
};

resource_serial_t *resource_serial_init_by_tag(
    const char *tag);

static inline void resource_serial_write(
    resource_serial_t *instance,
    void *buf,
    int bytes)
{
    instance->write(instance, buf, bytes);
}