#include "core/interface.h"
#include "lib/strops.h"
#include <stdlib.h>

interface_resource_t *interface_resource_allocate(
    const peripheral_instance_decl_t *peripheral,
    const char **argp)
{
    int rsc_count = peripheral->decl->num_rscs;
    int i;
    interface_resource_t *rscs = malloc(sizeof(interface_resource_t) * rsc_count);
    if (rscs == NULL) {
        return NULL;
    }

    /* Fetch resources */
    for (i = 0; i < rsc_count; i++) {
        const char *arg = strops_next_word(argp);
        interface_resource_t *rsc = &rscs[i];
        rsc->decl = resource_get_by_tag(arg);
        rsc->inst = peripheral_get_resource_by_tag(peripheral, i, arg);
        if (rsc->decl == NULL || rsc->inst == NULL) {
            return NULL;
        }
    }

    /* Allocate resources */
    for (i = 0; i < rsc_count; i++) {
        interface_resource_t *rsc = &rscs[i];
        if (!resource_allocate(rsc->decl)) {
            /* TODO: Properly handle if resources could not be allocated... */
            return NULL;
        }
    }

    return rscs;
}

interface_header_t *interface_create(
    const char *config)
{
    const char *cur_conf = config;

    const peripheral_instance_decl_t *decl = peripheral_get_by_tag(strops_next_word(&cur_conf));
    if (decl == NULL) {
        return NULL;
    }

    interface_header_t *iface = malloc(decl->decl->storage_size);
    if (iface == NULL) {
        /* TODO: error handling */
        asm("bkpt 255");
        return NULL;
    }
    iface->peripheral = decl;
    iface->rscs = interface_resource_allocate(decl, &cur_conf);
    if (iface->rscs == NULL) {
        /* TODO: error handling */
        asm("bkpt 255");
        return NULL;
    }
    if (decl->decl->init(iface, cur_conf) < 0) {
        /* TODO: error handling */
        asm("bkpt 255");
        return NULL;
    }

    return iface;
}