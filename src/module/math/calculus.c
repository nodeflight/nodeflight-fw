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

#include "core/scheduler.h"
#include "core/module.h"
#include "core/variable.h"

#include "FreeRTOS.h"
#include "task.h"

typedef struct integrate_s integrate_t;

struct integrate_s {
    float *in;
    float out;
    float period_sec;
};

/* Common for all binary operations */

static void integrate_sch_init(
    float period_sec,
    void *storage)
{
    integrate_t *integrate = storage;
    integrate->period_sec = period_sec;
}
static void integrate_sch_run(
    void *storage)
{
    integrate_t *integrate = storage;
    integrate->out += integrate->period_sec * *integrate->in;
}
static int integrate_init(
    const md_decl_t *md,
    const char *name,
    md_arg_t *args)
{
    integrate_t *integrate = pvPortMalloc(sizeof(integrate_t));
    if (integrate == NULL) {
        return -1;
    }

    integrate->out = 0.0f;
    integrate->period_sec = 0.0f;
    vr_register(name, md->outputs, &integrate->out);

    integrate->in = NULL;
    vr_request(args[1].name, VR_TYPE_FLOAT, (void **) &integrate->in);

    sc_register_client(args[0].sched, integrate_sch_init, integrate_sch_run, integrate);
    return 0;
}
MD_DECL(int_f, integrate_init,
    MD_DECL_ARGS(
        MD_ARG_DECL("schedule", MD_ARG_MODE_NORMAL, MD_ARG_TYPE_SCHEDULER, SC_DIR_IN),
        MD_ARG_DECL("in", MD_ARG_MODE_NORMAL, MD_ARG_TYPE_LINK, VR_TYPE_FLOAT)
    ),
    MD_DECL_OUTPUTS(
        MD_OUTPUT_DECL("out", VR_TYPE_FLOAT)
    )
);
