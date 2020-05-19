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
#include "FreeRTOS.h"

#include <stddef.h>

typedef struct servo_s servo_t;

struct servo_s {
    if_pwm_t *if_pwm;
};

static int servo_init(
    const char *tag);

MD_DECL(servo, servo_init);

int servo_init(
    const char *tag)
{
    servo_t *servo;
    const char *pp_config;
    int status;

    pp_config = config_get_pp_config(tag);
    if (pp_config == NULL) {
        return -1;
    }

    servo = pvPortMalloc(sizeof(servo_t));
    if (servo == NULL) {
        return -1;
    }

    servo->if_pwm = IF_PWM(if_create(pp_config, PP_PWM));
    if (servo->if_pwm == NULL) {
        return -1;
    }

    status = servo->if_pwm->configure(servo->if_pwm, &(const if_pwm_config_t) {
        .clock_hz = 1000000,
        .period_ticks = 20000,
        .pulses_count = 1,
        .pulses_cycle = 1
    });
    if (status != 0) {
        return -1;
    }

    status = servo->if_pwm->set_output(servo->if_pwm, (const uint16_t[]) { 1200 });
    if (status != 0) {
        return -1;
    }

    return 0;
}
