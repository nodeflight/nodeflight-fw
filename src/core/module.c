#include "core/module.h"
#include "core/strops.h"

const extern module_decl_t __nf_module_start[];
const extern module_decl_t __nf_module_end[];

int module_init(
    const char *name,
    const char *config)
{
    const module_decl_t *cur;
    for (cur = __nf_module_start; cur < __nf_module_end; cur++) {
        if (strops_word_cmp(cur->name, name) == 0) {
            return cur->init(config);
        }
    }
    return -1;
}