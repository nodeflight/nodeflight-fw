/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) Max Sikström
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * The platform of the native target: nodeflight as a process on the build
 * machine, on the POSIX port of FreeRTOS.
 *
 * There is no flash: the internal disk, the region __l1conf_start to
 * __l1conf_end of native.ld, is loaded from the image file that the
 * environment variable NODEFLIGHT_L1CONF names. Without it, the region stays
 * empty: the configuration fails to load, before any log output is set up,
 * and the main task stops.
 */

#include "platform/platform.h"

#include "FreeRTOS.h"
#include "task.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include <unistd.h>

#define L1CONF_ENV "NODEFLIGHT_L1CONF"

/* The internal disk, defined by native.ld. Read-only for the rest of the core */
extern uint8_t __l1conf_start[];
extern uint8_t __l1conf_end[];

static void platform_l1conf_load(
    void);

void platform_init(
    void)
{
    platform_l1conf_load();
}

/**
 * Load the image of the internal disk from the file named by NODEFLIGHT_L1CONF
 *
 * Exits the process if the file cannot be read, or is larger than the region.
 */
static void platform_l1conf_load(
    void)
{
    const char *path;
    size_t size;
    size_t len;
    FILE *f;

    size = (size_t) (__l1conf_end - __l1conf_start);

    path = getenv(L1CONF_ENV);
    if (path == NULL) {
        fprintf(stderr, "[config error]: Missing environment variable %s, internal disk empty\n", L1CONF_ENV);
        return;
    }

    f = fopen(path, "rb");
    if (f == NULL) {
        fprintf(stderr, "[io error]: Cannot open l1conf image %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    len = fread(__l1conf_start, 1, size, f);
    if (ferror(f)) {
        fprintf(stderr, "[io error]: Cannot read l1conf image %s: %s\n", path, strerror(errno));
        fclose(f);
        exit(EXIT_FAILURE);
    }
    if (len == size && fgetc(f) != EOF) {
        fprintf(stderr, "[io error]: Invalid l1conf image %s, larger than %zu bytes\n", path, size);
        fclose(f);
        exit(EXIT_FAILURE);
    }
    fclose(f);
}

uint32_t platform_random_get(
    void)
{
    uint32_t value;
    ssize_t len;

    do {
        len = getrandom(&value, sizeof(value), 0);
    } while (len < 0 && errno == EINTR);

    if (len != sizeof(value)) {
        fprintf(stderr, "[io error]: Cannot read random number: %s\n", strerror(errno));
        abort();
    }
    return value;
}

/**
 * Idle hook of FreeRTOS: sleep until the next tick
 *
 * Without it the idle task keeps a core of the host busy. The tick of the
 * POSIX port is a signal to the running thread, which ends pause().
 */
void vApplicationIdleHook(
    void)
{
    pause();
}
