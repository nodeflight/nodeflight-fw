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
#include "core/interface_types.h"

#include "FreeRTOS.h"
#include "task.h"

#define SCH_TIMER_PERIOD_TICKS 1000UL

typedef struct sch_timer_s sch_timer_t;

struct sch_timer_s {
    if_pwm_t *if_timer;
    sc_t *scheduler;
};

static int sch_timer_init(
    const char *name,
    md_arg_t *args);

void sch_timer_trigger(
    uint32_t *values,
    void *storage);

MD_DECL(sch_timer, "psci", sch_timer_init);

int sch_timer_init(
    const char *name,
    md_arg_t *args)
{
    sch_timer_t *schtm;
    int status;

    if (args[0].iface->peripheral->decl->type != PP_PWM) {
        return -1;
    }

    schtm = pvPortMalloc(sizeof(sch_timer_t));
    if (schtm == NULL) {
        return -1;
    }

    schtm->if_timer = IF_PWM(args[0].iface);
    schtm->scheduler = args[1].sched;

    sc_configure_source(schtm->scheduler, 1.0f / args[2].const_int);

    status = schtm->if_timer->configure(schtm->if_timer, &(const if_pwm_cf_t) {
        .clock_hz = SCH_TIMER_PERIOD_TICKS * args[2].const_int,
        .period_ticks = SCH_TIMER_PERIOD_TICKS,
        .pulses_count = 1,
        .update_values_cb = sch_timer_trigger,
        .storage = schtm
    });
    if (status != 0) {
        return -1;
    }

    return 0;
}

void sch_timer_trigger(
    uint32_t *values,
    void *storage)
{
    sch_timer_t *schtm = storage;
    *values = 0;
    sc_trigger_from_isr(schtm->scheduler);
}
