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

#include "core/scheduler.h"
#include "core/module.h"
#include "core/variable.h"

#include "FreeRTOS.h"
#include <stddef.h>

typedef struct sch_decim_s sch_decim_t;

struct sch_decim_s {
    sc_t *src_sched;
    sc_t *tgt_sched;
    int32_t decimation_factor;
    int32_t current_counter;
};

static int sch_decim_init(
    const char *name,
    md_arg_t *args);

static void sch_decim_src_init(
    float period_sec,
    void *storage);

static void sch_decim_src_run(
    void *storage);

MD_DECL(sch_decim, sch_decim_init,
    MD_DECL_ARGS(
        MD_ARG_DECL("schedule_in", MD_ARG_MODE_NORMAL, MD_ARG_TYPE_SCHEDULER, SC_DIR_IN),
        MD_ARG_DECL("schedule_out", MD_ARG_MODE_NORMAL, MD_ARG_TYPE_SCHEDULER, SC_DIR_OUT),
        MD_ARG_DECL("decimation_factor", MD_ARG_MODE_NORMAL, MD_ARG_TYPE_CONST, VR_TYPE_INT),
        MD_ARG_DECL("out_frequency", MD_ARG_MODE_NORMAL, MD_ARG_TYPE_CONST, VR_TYPE_INT)
    )
);

int sch_decim_init(
    const char *name,
    md_arg_t *args)
{
    sch_decim_t *decim;

    decim = pvPortMalloc(sizeof(sch_decim_t));
    if (decim == NULL) {
        return -1;
    }

    decim->src_sched = args[0].sched;
    decim->tgt_sched = args[1].sched;
    decim->decimation_factor = args[2].const_int;
    decim->current_counter = decim->decimation_factor;

    /* TODO: Determine frequency from source */
    sc_configure_source(decim->tgt_sched, 1.0f / args[3].const_int);

    sc_register_client(decim->src_sched, sch_decim_src_init, sch_decim_src_run, decim);

    return 0;
}

void sch_decim_src_init(
    float period_sec,
    void *storage)
{
    /* Do nothing for now */
}

void sch_decim_src_run(
    void *storage)
{
    sch_decim_t *decim = storage;
    decim->current_counter--;
    if (decim->current_counter == 0) {
        decim->current_counter = decim->decimation_factor;
        sc_trigger(decim->tgt_sched);
    }
}
