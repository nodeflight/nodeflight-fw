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
#include "core/peripheral.h"
#include "core/config.h"
#include "core/interface_types.h"
#include "core/scheduler.h"
#include "core/variable.h"

#include "FreeRTOS.h"
#include "task.h"

#include "core/log.h"

#include <stddef.h>

#define PWM_CLOCK_HZ      2048000UL                /**< source clock frequency, ticks per second */
#define PWM_FREQUENCY_HZ  50UL                     /**< pulses per second */
#define PWM_MS            (PWM_CLOCK_HZ / 1000UL)  /**< ticks per millisecond */

typedef struct servo_s servo_t;

struct servo_s {
    if_pwm_t *if_pwm;

    TaskHandle_t task;

    /* Value for output buffer */
    uint32_t raw_value;

    /* Source value */
    float *f_value;
};

static int servo_init(
    const char *name,
    md_arg_t *args);

static void servo_new_values(
    uint32_t *values,
    void *storage);

static void servo_sched_init(
    float period_sec,
    void *storage);

static void servo_sched_run(
    void *storage);

MD_DECL(
    servo,
    "psn",
    servo_init);

int servo_init(
    const char *name,
    md_arg_t *args)
{
    if (args[0].iface->peripheral->decl->type != PP_PWM) {
        return -1;
    }

    servo_t *servo;
    int status;

    servo = pvPortMalloc(
        sizeof(servo_t));
    if (servo == NULL) {
        return -1;
    }

    servo->if_pwm = IF_PWM(
        args[0].iface);

    status = servo->if_pwm->configure(
        servo->if_pwm,
        &(const if_pwm_cf_t) {
        .clock_hz = PWM_CLOCK_HZ,
        .period_ticks = (PWM_CLOCK_HZ / PWM_FREQUENCY_HZ),
        .pulses_count = 1,
        .update_values_cb = servo_new_values,
        .storage = servo
    });
    if (status != 0) {
        return -1;
    }

    servo->raw_value = (3 * PWM_MS) / 2;
    servo->f_value = NULL;

    vr_request(args[2].name, 'f', (void **) &servo->f_value);

    sc_register_client(
        args[1].sched,
        servo_sched_init,
        servo_sched_run,
        servo);

    return 0;
}

void servo_new_values(
    uint32_t *values,
    void *storage)
{
    servo_t *servo = storage;
    values[0] = servo->raw_value;
}

void servo_sched_init(
    float period_sec,
    void *storage)
{
    servo_t *servo = storage;
    (void) servo;
}

void servo_sched_run(
    void *storage)
{
    servo_t *servo = storage;
    float value = *servo->f_value;

    if (value < -1.0f) {
        value = -1.0f;
    }
    if (value > 1.0f) {
        value = 1.0f;
    }
    value = (1.0f + value) / 2.0f;
    servo->raw_value = PWM_MS + (uint32_t) (value * PWM_MS);
}
