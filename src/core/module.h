#pragma once

typedef struct module_decl_s module_decl_t;

struct module_decl_s {
    const char *name;
    int (*init)(
        const char *config);
};

#define _MODULE_SECTION(_name) __attribute__ ((section(".nf_module." #_name), used))

#define MODULE_DECL(_name, _init) \
    const module_decl_t module_ ## _name ## _decl _MODULE_SECTION(_name) = { \
        .name = #_name, \
        .init = _init \
    }

int module_init(
    const char *name,
    const char *config);