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
    char *const *pp_argv = cf_get_pp_config(tag);
    if (pp_argv == NULL) {
        return NULL;
    }
    int pp_argc = strops_get_argc(pp_argv);

    return if_create(pp_argc, pp_argv, PP_NONE);
}

static int md_init_mod(
    const md_decl_t *md,
    const char *name,
    int argc,
    char **argv)
{
    md_arg_t args[MD_MAX_ARGS];
    const char *argstrptr = &md->args[0];

    int curarg = 0;

    while (*argstrptr != '\0' && curarg < argc) {
        bool optional = (*argstrptr == '?');
        if (optional) {
            argstrptr++;
        }

        if (optional && 0 == strops_cmp("-", argv[curarg])) {
            args[curarg].iface = NULL; /* Union, all optionals are pointers, everything will be NULL */
        } else {
            switch (*argstrptr) {
            case 'p':
                args[curarg].iface = md_load_interface(argv[curarg]);
                if (args[curarg].iface == NULL) {
                    return -1;
                }
                break;

            case 's':
                args[curarg].sched = sc_get(argv[curarg]);
                if (args[curarg].sched == NULL) {
                    return -1;
                }
                break;

            case 'n':
                args[curarg].name = argv[curarg];
                break;

            case 'c':
                argstrptr++;
                /* Constant, always followed by type char */
                switch (*argstrptr) {
                case 'i':     /* int32_t */
                    args[curarg].const_int = strops_word_to_int(argv[curarg]);
                    break;

                case 'f':     /* float */
                    args[curarg].const_float = strops_word_to_float(argv[curarg]);
                    break;

                default:
                    return -1;
                }
                break;

            default:
                return -1;
            }
        }

        argstrptr++;
        curarg++;
    }

    if (curarg != argc) {
        return -1;
    }

    return md->init(name, args);
}

int md_init(
    const char *mdname,
    const char *name,
    int argc,
    char **argv)
{
    const md_decl_t *md;
    for (md = __nf_module_start; md < __nf_module_end; md++) {
        if (strops_cmp(md->name, mdname) == 0) {
            return md_init_mod(md, name, argc, argv);
        }
    }
    return -1;
}

const md_decl_t *md_get_decl_by_id(
    int index)
{
    if (index < 0 || &__nf_module_start[index] >= __nf_module_end) {
        return NULL;
    }
    return &__nf_module_start[index];
}

int md_get_decl_count(
    void)
{
    return __nf_module_end - __nf_module_start;
}
