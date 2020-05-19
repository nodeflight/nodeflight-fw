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

#include "stm32.h"
#include "core/peripheral.h"
#include "core/resource.h"
#include "core/interface_types.h"
#include "platform/stm32/hwacc/timer.h"
#include "platform/stm32/hwacc/gpio.h"

#include "FreeRTOS.h"

#include "vendor/tinyprintf/tinyprintf.h"

#include <stddef.h>

typedef struct timer_if_s timer_if_t;

struct timer_if_s {
    if_pwm_t header;
    timer_def_t def;

    uint8_t dummy;
};

static int timer_init(
    if_header_t *iface,
    const char *config);

static int timer_configure(
    if_pwm_t *iface,
    const if_pwm_config_t *config);

static int timer_set_output(
    if_pwm_t *iface,
    const uint16_t *widths);

PP_TYPE_DECL(
    timer,
    PP_PWM,
    TIMER_NUM_ARGS,
    timer_init,
    sizeof(timer_if_t));

int timer_init(
    if_header_t *iface,
    const char *config)
{
    timer_if_t *if_pwm = (timer_if_t *) iface;

    if_pwm->header.configure = timer_configure;
    if_pwm->header.set_output = timer_set_output;

    if_pwm->def = *((const timer_def_t *) if_pwm->header.header.peripheral->storage);
    return 0;
}

int timer_configure(
    if_pwm_t *iface,
    const if_pwm_config_t *config)
{
    timer_if_t *if_pwm = (timer_if_t *) iface;
    if_rs_t *rscs = if_pwm->header.header.rscs;
    /* TIM6 CH1 A00 AF2 */

    /* TODO: Keep state for when reusing timers for other channels */
    LL_TIM_Init(if_pwm->def.reg, &(LL_TIM_InitTypeDef) {
        .Prescaler = (SystemCoreClock / config->clock_hz) - 1, /* Prescale down to 1us */
        .CounterMode = LL_TIM_COUNTERMODE_UP,
        .Autoreload = config->period_ticks,
        .ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
        .RepetitionCounter = 0
    });

    LL_TIM_OC_Init(if_pwm->def.reg, if_pwm->def.channel, &(LL_TIM_OC_InitTypeDef) {
        .OCMode = LL_TIM_OCMODE_PWM1,
        .CompareValue = 0,
        .OCState = LL_TIM_OCSTATE_ENABLE,
        .OCPolarity = LL_TIM_OCPOLARITY_HIGH,
        .OCIdleState = LL_TIM_OCIDLESTATE_LOW,
        .OCNState = LL_TIM_OCSTATE_DISABLE,
        .OCNPolarity = LL_TIM_OCPOLARITY_LOW,
        .OCNIdleState = LL_TIM_OCIDLESTATE_LOW
    });

    gpio_config_by_id(rscs[TIMER_ARG_PIN].decl->ref, &(LL_GPIO_InitTypeDef) {
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = rscs[TIMER_ARG_PIN].inst->attr
    });

    LL_TIM_OC_EnablePreload(if_pwm->def.reg, if_pwm->def.channel);
    LL_TIM_OC_DisableFast(if_pwm->def.reg, if_pwm->def.channel);

    LL_TIM_EnableCounter(if_pwm->def.reg);
    // LL_EX_TIM_CC_EnableNChannel(timer, channel);
    LL_TIM_CC_EnableChannel(if_pwm->def.reg, if_pwm->def.channel);

    LL_TIM_EnableAllOutputs(if_pwm->def.reg);
    LL_TIM_EnableARRPreload(if_pwm->def.reg);
    LL_TIM_EnableCounter(if_pwm->def.reg);

    LL_TIM_EnableAllOutputs(if_pwm->def.reg);

    return 0;
}

static int timer_set_output(
    if_pwm_t *iface,
    const uint16_t *widths)
{
    timer_if_t *if_pwm = (timer_if_t *) iface;
    switch (if_pwm->def.channel) {
    case LL_TIM_CHANNEL_CH1:
        LL_TIM_OC_SetCompareCH1(if_pwm->def.reg, widths[0]);
        break;

    case LL_TIM_CHANNEL_CH2:
        LL_TIM_OC_SetCompareCH2(if_pwm->def.reg, widths[0]);
        break;

    case LL_TIM_CHANNEL_CH3:
        LL_TIM_OC_SetCompareCH3(if_pwm->def.reg, widths[0]);
        break;

    case LL_TIM_CHANNEL_CH4:
        LL_TIM_OC_SetCompareCH4(if_pwm->def.reg, widths[0]);
        break;

    case LL_TIM_CHANNEL_CH5:
        LL_TIM_OC_SetCompareCH5(if_pwm->def.reg, widths[0]);
        break;

    case LL_TIM_CHANNEL_CH6:
        LL_TIM_OC_SetCompareCH6(if_pwm->def.reg, widths[0]);
        break;
    }
    return 0;
}
