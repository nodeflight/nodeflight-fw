#include "peripheral/peripheral.h"

#include <string.h>
#include <stdlib.h>

const extern peripheral_instance_decl_t __nf_peripheral_start[];
const extern peripheral_instance_decl_t __nf_peripheral_end[];

const static char *type_names[NUM_PERIPHERAL_TYPE] = {
    [PERIPHERAL_NONE] = "NONE",
    [PERIPHERAL_SERIAL] = "SERIAL"
};

const char *peripheral_get_type_name(
    peripheral_type_t type)
{
    return type_names[type];
}

const peripheral_instance_decl_t *peripheral_get_by_index(
    int index)
{
    return &__nf_peripheral_start[index];
}

const int peripheral_get_count(
    void)
{
    return __nf_peripheral_end - __nf_peripheral_start;
}

const peripheral_instance_decl_t *peripheral_get_by_tag(
    peripheral_type_t type,
    const char *tag)
{
    const peripheral_instance_decl_t *cur;
    for (cur = __nf_peripheral_start; cur < __nf_peripheral_end; cur++) {
        if (cur->decl->type == type && 0 == strcmp(cur->tag, tag)) {
            return cur;
        }
    }
    return NULL;
}

peripheral_serial_t *peripheral_serial_init_by_tag(
    const char *tag)
{
    const peripheral_instance_decl_t *variant = peripheral_get_by_tag(PERIPHERAL_SERIAL, tag);
    if (variant == NULL) {
        return NULL;
    }
    peripheral_serial_t *inst = malloc(variant->decl->storage_size);
    if (inst == NULL) {
        return NULL;
    }
    memcpy(inst, variant->decl->callbacks, sizeof(peripheral_serial_t));

    inst->init(inst, variant);

    return inst;
}