#include "core/stdout.h"
#include "hwacc/peripheral.h"

#include <stdint.h>
#include <stddef.h>

peripheral_serial_t *stdout_instance;

int _write(
    int fd,
    uint8_t *buf,
    int size)
{
    /* stdout */
    if (fd == 1) {
        if(stdout_instance != NULL) {
            peripheral_serial_write(stdout_instance, buf, size);
        }
        return size;
    }
    return -1;
}

int stdout_init(
    const char *tag)
{
    stdout_instance = peripheral_serial_init_by_tag(tag);
    if (stdout_instance == NULL) {
        return -1;
    }

    return 0;
}