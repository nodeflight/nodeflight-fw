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

#include "vendor/tinyprintf/tinyprintf.h"

#define LINEBUF_SIZE 256
#define LINEBUF_NUM_ARGS 32

extern const char __l1conf_start[];

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
    } else {
        /* TODO: Error handling */
        return -1;
    }
    return 0;
}

int cf_init(
    void)
{
    const char *cur = __l1conf_start;

    cf_peripherals = map_create();

    while (*cur != '\0') {
        if (0 == strops_line_copy(cf_linebuf, LINEBUF_SIZE, &cur)) {
            char *argv[LINEBUF_NUM_ARGS];
            int argc = strops_split_argv(cf_linebuf, argv);
            if (cf_process_line(argc, argv) < 0) {
                /* TODO: Error handling */
                return -1;
            }
        }
    }

    return 0;
}

char *const *cf_get_pp_config(
    const char *tag)
{
    return map_get(cf_peripherals, tag);
}
