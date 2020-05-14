#pragma once

#include "hwacc/interface.h"

typedef struct interface_serial_s interface_serial_t;

struct interface_serial_s {
    interface_header_t header;

    int (*tx_write)(
        interface_serial_t *iface,
        const void *buf,
        int bytes);

    void (*tx_wait_done)(
        interface_serial_t *iface);
};

static inline int interface_serial_tx_write(
    interface_header_t *iface,
    void *buf,
    int bytes)
{
    if (iface->peripheral->decl->type != PERIPHERAL_SERIAL) {
        return -1;
    }
    interface_serial_t *if_ser = (interface_serial_t *) iface;
    return if_ser->tx_write(if_ser, buf, bytes);
}

static inline void interface_serial_tx_wait_done(
    interface_header_t *iface)
{
    if (iface->peripheral->decl->type != PERIPHERAL_SERIAL) {
        return;
    }
    interface_serial_t *if_ser = (interface_serial_t *) iface;
    if_ser->tx_wait_done(if_ser);
}