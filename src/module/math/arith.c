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
#include "task.h"

typedef struct arith_s arith_t;

struct arith_s {
    float *in[2];
    float out;
};

/* Common for all binary operations */

static void arith_sch_init(
    float period_sec,
    void *storage)
{
    /* Do nothing */
}
static int arith_init(
    const char *name,
    md_arg_t *args,
    sc_clinet_run_t op,
    int num_args)
{
    arith_t *arith;
    int i;

    arith = pvPortMalloc(sizeof(arith_t));
    if (arith == NULL) {
        return -1;
    }

    arith->out = 0.0f;

    vr_register(name, "f", &arith->out);
    for (i = 0; i < num_args; i++) {
        arith->in[i] = NULL;
        vr_request(args[1 + i].name, 'f', (void **) &arith->in[i]);
    }

    sc_register_client(args[0].sched, arith_sch_init, op, arith);
    (void)arith_sch_init;
    return 0;
}

/* Op: add */

static void arith_sch_run_add(
    void *storage)
{
    arith_t *arith = storage;
    if (arith->in[0] == NULL || arith->in[1] == NULL) {
        return;
    }
    arith->out = *arith->in[0] + *arith->in[1];
}
static int arith_init_add(
    const char *name,
    md_arg_t *args)
{
    return arith_init(name, args, arith_sch_run_add, 2);
}
MD_DECL(add_f, "snn", arith_init_add);

/* Op: multiply */

static void arith_sch_run_mul(
    void *storage)
{
    arith_t *arith = storage;
    if (arith->in[0] == NULL || arith->in[1] == NULL) {
        return;
    }
    arith->out = *arith->in[0] * *arith->in[1];
}
static int arith_init_mul(
    const char *name,
    md_arg_t *args)
{
    return arith_init(name, args, arith_sch_run_mul, 2);
}
MD_DECL(mul_f, "snn", arith_init_mul);

/* Op: subtract */

static void arith_sch_run_sub(
    void *storage)
{
    arith_t *arith = storage;
    if (arith->in[0] == NULL || arith->in[1] == NULL) {
        return;
    }
    arith->out = *arith->in[0] - *arith->in[1];
}
static int arith_init_sub(
    const char *name,
    md_arg_t *args)
{
    return arith_init(name, args, arith_sch_run_sub, 2);
}
MD_DECL(sub_f, "snn", arith_init_sub);

/* Op: divide */

static void arith_sch_run_div(
    void *storage)
{
    arith_t *arith = storage;
    if (arith->in[0] == NULL || arith->in[1] == NULL) {
        return;
    }
    arith->out = *arith->in[0] / *arith->in[1];
}
static int arith_init_div(
    const char *name,
    md_arg_t *args)
{
    return arith_init(name, args, arith_sch_run_div, 2);
}
MD_DECL(div_f, "snn", arith_init_div);

/* Op negate */

static void arith_sch_run_neg(
    void *storage)
{
    arith_t *arith = storage;
    if (arith->in[0] == NULL) {
        return;
    }
    arith->out = -*arith->in[0];
}
static int arith_init_neg(
    const char *name,
    md_arg_t *args)
{
    return arith_init(name, args, arith_sch_run_neg, 1);
}
MD_DECL(neg_f, "sn", arith_init_neg);
