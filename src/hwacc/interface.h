#pragma once

#include <stdint.h>

typedef struct interface_resource_s interface_resource_t;
typedef struct interface_header_s interface_header_t;

#include "hwacc/peripheral.h"
#include "hwacc/resource.h"

struct interface_resource_s {
    const resource_decl_t *decl;
    const peripheral_instance_resource_t *inst;
};

/**
 * Interface header
 *
 * Each inteface has its storage. All interfaces has in common that the first argument
 * is this header, which means all interface pointers can be type cast to the header to
 * determine its type.
 */
struct interface_header_s {
    const peripheral_instance_decl_t *peripheral;
    interface_resource_t *rscs; /* resources in use */
};

interface_resource_t *interface_resource_allocate(
    const peripheral_instance_decl_t *peripheral,
    const char **argp);

interface_header_t *interface_create(
    const char *config);
