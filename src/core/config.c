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
#include "lib/strops.h"
#include "lib/map.h"

#define LINEBUF_SIZE 256

extern const char __l1conf_start[];

static char config_linebuf[LINEBUF_SIZE];
static map_t *config_per;

static void config_parse_line(
    const char *line)
{
    const char *command = strops_next_word(&line);
    if (*command == '\0' || *command == '#') {
        /* Ignore empty lines and comments */
    } else if (0 == strops_word_cmp("mod", command)) {
        /* Load module */
        const char *name = strops_next_word(&line);
        if (0 != module_init(name, line)) {
            /* TODO: Error handling */
        }
    } else if (0 == strops_word_cmp("per", command)) {
        /* Load peripheral definition */
        const char *name = strops_next_word(&line);
        map_set(config_per, name, strops_line_dup(line));
    } else {
        /* TODO: Error handling */
    }
}

void config_init(
    void)
{
    const char *cur = __l1conf_start;

    config_per = map_create();

    while (*cur != '\0') {
        if (0 == strops_line_copy(config_linebuf, LINEBUF_SIZE, &cur)) {
            config_parse_line(config_linebuf);
        }
    }
}

const char *config_get_peripheral_config(
    const char *tag)
{
    return map_get(config_per, tag);
}
