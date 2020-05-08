#pragma once

#include <stdint.h>

/**
 * Interface type
 *
 * The type models the method of access, not the external behaviour. If it modelled as a
 * serial port which handles stream-based IO, it is a serial port independent of being an UART
 * or TCP. Is is a value_out port independent of being a DSHOT, PWM or DAC output.
 */

typedef enum interface_type_s {
    INTERFACE_NONE = 0,
    INTERFACE_SERIAL
} interface_type_t;

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
    interface_type_t type;
    uint8_t num_rscs;
    interface_resource_t *rscs; /* resources in use */
};

interface_resource_t *interface_resource_allocate(
    const peripheral_instance_decl_t *peripheral,
    const char **argp);

interface_header_t *interface_create(
    const char *config);
