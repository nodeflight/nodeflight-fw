/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2020  Max Sikström
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

#include "core/config.h"
#include "core/module.h"
#include "core/scheduler.h"
#include "lib/strops.h"
#include "lib/map.h"

#include "ff.h"

#include "vendor/tinyprintf/tinyprintf.h"

#define LINEBUF_SIZE 256
#define LINEBUF_NUM_ARGS 32

static char cf_linebuf[LINEBUF_SIZE];
static map_t *cf_peripherals;

static int cf_process_line(
    int argc,
    char **argv)
{
    if (argc == 0) {
        /* Ignore empty lines and comments */
    } else if (argv[0][0] == '#') {
        /* Comment, ignore */

    } else if (argc >= 2 && 0 == strops_cmp("per", argv[0])) {
        /* Load peripheral definition */
        map_set(cf_peripherals, argv[1], strops_argv_dup(&argv[2]));

    } else if (argc == 3 && 0 == strops_cmp("sch", argv[0])) {
        if (NULL == sc_define(argv[1], strops_word_to_int(argv[2]))) {
            /* TODO: Error handling */
            return -1;
        }

    } else if (argc >= 3 && 0 == strops_cmp("mod", argv[0])) {
        /* Load module */
        char *name = argv[1];

        /* Name is optional */
        if (strops_cmp("-", name) == 0) {
            name = NULL;
        }
        if (md_init(argv[2], name, argc - 3, &argv[3]) != 0) {
            /* TODO: Error handling */
            return -1;
        }
    } else if (0 == strops_cmp("inc", argv[0])) {
        /* Ignore for now, enqueue file name later */
    } else {
        /* TODO: Error handling */
        return -1;
    }
    return 0;
}

int cf_init(
    const char *path)
{
    FIL f;
    FRESULT res;

    cf_peripherals = map_create();

    res = f_open(&f, path, FA_READ);
    if (res != FR_OK) {
        return -1;
    }

    /* Read every line and display it */
    while (f_gets(cf_linebuf, sizeof(cf_linebuf), &f)) {
        char *argv[LINEBUF_NUM_ARGS];
        int argc = strops_split_argv(cf_linebuf, argv);
        if (cf_process_line(argc, argv) < 0) {
            f_close(&f);
            return -1;
        }
    }
    f_close(&f);

    return 0;
}

char *const *cf_get_pp_config(
    const char *tag)
{
    return map_get(cf_peripherals, tag);
}
