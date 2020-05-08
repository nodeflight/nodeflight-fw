#include "core/stdout.h"
#include "hwacc/interface.h"
#include "hwacc/interface_types.h"

#include <stdint.h>
#include <stddef.h>

interface_header_t *if_stdout;

int _write(
    int fd,
    uint8_t *buf,
    int size)
{
    /* stdout */
    if (fd == 1) {
        if(if_stdout != NULL) {
            interface_serial_write(if_stdout, buf, size);
        }
        return size;
    }
    return -1;
}

int stdout_init(
    const char *tag)
{
    if_stdout = interface_create(tag);
    if (if_stdout == NULL) {
        return -1;
    }

    return 0;
}