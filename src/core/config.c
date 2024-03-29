/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2023  Max Sikström
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

#define DEBUG_CONFIG                1

#if DEBUG_CONFIG
#include "core/log.h"
#define D_CONFIG_PRINTLN(...) log_println(__VA_ARGS__);
#else
#define D_CONFIG_PRINTLN(...) do {} while(0)
#endif

#define CONFIG_PATH_QUEUE_LENGTH 16

#define LINEBUF_SIZE 256
#define LINEBUF_NUM_ARGS 32

static const char *cf_path_queue[CONFIG_PATH_QUEUE_LENGTH];
static int cf_queue_head;
static int cf_queue_tail;

static char cf_linebuf[LINEBUF_SIZE];
static map_t *cf_peripherals;

static int cf_process_line(
    int argc,
    char **argv)
{
    if (argc == 0) {
        /* Ignore empty lines and comments */
    } else {
#if DEBUG_CONFIG
        /* Log config line. Debug output should be loaded as early as possible if available. Anything before that will
         * be discarded from output */
        /* Don't use D_CONFIG_PRINTF, since it won't handle multiple writes on single line */
        {
            int i;
            log_line_t line;
            log_begin_line(&line);
            for (i = 0; i < argc; i++) {
                log_printf(&line, " %s", argv[i]);
            }
            log_finish_line(&line);
        }
#endif

        if (argv[0][0] == '#') {
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
        } else if (argc == 2 && 0 == strops_cmp("inc", argv[0])) {
            /* Push config file path to queue */
            cf_path_queue[cf_queue_tail] = strops_dup(argv[1]);
            cf_queue_tail = (cf_queue_tail + 1) % CONFIG_PATH_QUEUE_LENGTH;
        } else {
            /* TODO: Error handling */
            return -1;
        }
    }
    return 0;
}

int cf_init(
    const char *path)
{
    FIL f;
    FRESULT res;

    cf_peripherals = map_create();

    /* Initialize queue and load initial path */
    cf_queue_head = 0;
    cf_queue_tail = 0;

    cf_path_queue[cf_queue_tail] = strops_dup(path);
    cf_queue_tail = (cf_queue_tail + 1) % CONFIG_PATH_QUEUE_LENGTH;

    /* Load config files */
    while (cf_queue_head != cf_queue_tail) {
        D_CONFIG_PRINTLN("%s", cf_path_queue[cf_queue_head]);
        res = f_open(&f, cf_path_queue[cf_queue_head], FA_READ);
        if (res != FR_OK) {
            return -1;
        }

        cf_queue_head = (cf_queue_head + 1) % CONFIG_PATH_QUEUE_LENGTH;

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
    }

    return 0;
}

char *const *cf_get_pp_config(
    const char *tag)
{
    return map_get(cf_peripherals, tag);
}
