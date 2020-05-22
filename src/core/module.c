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

#include "core/module.h"
#include "core/config.h"
#include "core/interface.h"
#include "core/scheduler.h"
#include "lib/strops.h"

#include <stddef.h>

#define MD_MAX_ARGS 16

const extern md_decl_t __nf_module_start[];
const extern md_decl_t __nf_module_end[];

static if_header_t *md_load_interface(
    const char *tag)
{
    const char *pp_config;

    pp_config = config_get_pp_config(tag);
    if (pp_config == NULL) {
        return NULL;
    }

    return if_create(pp_config, PP_NONE);
}

static int md_init_mod(
    const md_decl_t *md,
    const char *config)
{
    md_arg_t args[MD_MAX_ARGS];
    const char *argptr = &md->args[0];
    int i = 0;
    while (*argptr != '\0') {
        const char *arg_str = strops_next_word(&config);

        bool optional = (*argptr == '?');
        if (optional) {
            argptr++;
        }

        if (optional && 0 == strops_word_cmp("-", arg_str)) {
            args[i].iface = NULL; /* Union, all optionals are pointers, everything will be NULL */
        } else {
            switch (*argptr) {
            case 'p':
                args[i].iface = md_load_interface(arg_str);
                if (args[i].iface == NULL) {
                    return -1;
                }
                break;

            case 's':
                args[i].sched = scheduler_get(arg_str);
                if (args[i].sched == NULL) {
                    return -1;
                }
                break;

            case 'c':
                argptr++;
                /* Constant, always followed by type char */
                switch (*argptr) {
                case 'i':     /* int32_t */
                    args[i].const_int = strops_word_to_int(arg_str);
                    break;

                case 'f':     /* float */
                    args[i].const_float = strops_word_to_float(arg_str);
                    break;

                default:
                    return -1;
                }
                break;

            default:
                return -1;
            }
        }

        argptr++;
        i++;
    }

    return md->init(args);
}

int md_init(
    const char *name,
    const char *config)
{
    const md_decl_t *md;
    for (md = __nf_module_start; md < __nf_module_end; md++) {
        if (strops_word_cmp(md->name, name) == 0) {
            return md_init_mod(md, config);
        }
    }
    return -1;
}
