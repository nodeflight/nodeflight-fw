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

extern const char __l1conf_start[];

static char cf_linebuf[LINEBUF_SIZE];
static map_t *cf_peripherals;

static void cf_parse_line(
    const char *line)
{
    const char *command = strops_next_word(&line);
    if (*command == '\0' || *command == '#') {
        /* Ignore empty lines and comments */
    } else if (0 == strops_word_cmp("per", command)) {
        /* Load peripheral definition */
        const char *name = strops_next_word(&line);
        map_set(cf_peripherals, name, strops_line_dup(line));
    } else if (0 == strops_word_cmp("sch", command)) {
        const char *name = strops_next_word(&line);
        const char *prio_str = strops_next_word(&line);
        if (*name != '\0' && *prio_str != '\0') {
            int priority = strops_word_to_int(prio_str);
            if (NULL == sc_define(name, priority)) {
                /* TODO: Error handling */
            }
        }
    } else if (0 == strops_word_cmp("mod", command)) {
        /* Load module */
        const char *name = strops_next_word(&line);
        const char *mdname = strops_next_word(&line);
        if (*name == '\0' || *mdname == '\0') {
            /* TODO: Error handling */
        } else {
            /* Name is optional */
            if (strops_word_cmp("-", name) == 0) {
                name = NULL;
            }
            int status = md_init(mdname, name, line);
            if (status != 0) {
                /* TODO: Error handling */
            }
        }
    } else {
        /* TODO: Error handling */
    }
}

void cf_init(
    void)
{
    const char *cur = __l1conf_start;

    cf_peripherals = map_create();

    while (*cur != '\0') {
        if (0 == strops_line_copy(cf_linebuf, LINEBUF_SIZE, &cur)) {
            cf_parse_line(cf_linebuf);
        }
    }
}

const char *cf_get_pp_config(
    const char *tag)
{
    return map_get(cf_peripherals, tag);
}
