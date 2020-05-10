#pragma once

#include "hwacc/interface.h"

typedef struct interface_serial_s interface_serial_t;

struct interface_serial_s {
    interface_header_t header;

    int (*write)(
        interface_serial_t *iface,
        void *buf,
        int bytes);
};

static inline int interface_serial_write(
    interface_header_t *iface,
    void *buf,
    int bytes)
{
    if (iface->peripheral->decl->type != PERIPHERAL_SERIAL) {
        return -1;
    }
    interface_serial_t *if_ser = (interface_serial_t *) iface;
    return if_ser->write(if_ser, buf, bytes);
}