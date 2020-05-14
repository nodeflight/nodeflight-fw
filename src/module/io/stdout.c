#include "core/module.h"
#include "hwacc/interface.h"
#include "hwacc/interface_types.h"

#include <stdint.h>
#include <stddef.h>

interface_header_t *if_stdout;

static int stdout_init(
    const char *tag);

MODULE_DECL(stdout, stdout_init);

int _write(
    int fd,
    uint8_t *buf,
    int size)
{
    /* stdout */
    if (fd == 1) {
        if (if_stdout != NULL) {
            interface_serial_tx_write(if_stdout, buf, size);
        }
        return size;
    }
    return -1;
}

int stdout_init(
    const char *tag)
{
    /* Can only have one stdout module loaded */
    if (if_stdout != NULL) {
        return -1;
    }

    if_stdout = interface_create(tag);
    if (if_stdout == NULL) {
        return -1;
    }

    return 0;
}