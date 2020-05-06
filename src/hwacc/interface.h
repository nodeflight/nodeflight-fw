#pragma once

#include "hwacc/peripheral.h"
#include "hwacc/resource.h"

typedef struct interface_resource_s interface_resource_t;

struct interface_resource_s {
    const resource_decl_t *decl;
    const peripheral_instance_resource_t *inst;
};

interface_resource_t *interface_resource_allocate(
    const peripheral_instance_decl_t *peripheral,
    const char **argp);