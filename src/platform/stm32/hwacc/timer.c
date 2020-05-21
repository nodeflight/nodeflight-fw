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
#include "platform/stm32/hwacc/dma.h"
#include "platform/stm32/hwacc/timer.h"
#include "platform/stm32/hwacc/gpio.h"

#include "FreeRTOS.h"

#include <stddef.h>

#define TIMER_DMA_IRQ_PRIORITY 9

typedef struct timer_s timer_t;
typedef struct timer_if_s timer_if_t;

/* Configuration of the timer main block. Multiple channels must use the same main block config */
struct timer_s {
    uint32_t clock_hz;
    uint32_t period_ticks;
};

struct timer_if_s {
    if_pwm_t header;
    timer_def_t def;

    const dma_stream_def_t *dma;
    timer_t *timer;

    uint32_t *pulses;
    uint16_t pulses_count;

    void (*update_values_cb)(
        uint32_t *values,
        void *storage);
    void *storage;
};

static timer_t timers[TIMER_MAX_COUNT] = {
    0
};

static int timer_init(
    if_header_t *iface,
    const char *config);

static int timer_configure(
    if_pwm_t *iface,
    const if_pwm_config_t *config);

static void timer_update_values_cb(
    const dma_stream_def_t *def,
    void *storage);

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

    if_pwm->def = *((const timer_def_t *) if_pwm->header.header.peripheral->storage);
    if_pwm->timer = &timers[if_pwm->def.timer_id];
    return 0;
}

static void timer_configure_main_block(
    timer_t *timer,
    timer_def_t *def,
    const if_pwm_config_t *config)
{
    timer->clock_hz = config->clock_hz;
    timer->period_ticks = config->period_ticks;

    /* TODO: Keep state for when reusing timers for other channels */
    LL_TIM_Init(def->reg, &(LL_TIM_InitTypeDef) {
        .Prescaler = (SystemCoreClock / timer->clock_hz) - 1, /* Prescale down to 1us */
        .CounterMode = LL_TIM_COUNTERMODE_UP,
        .Autoreload = timer->period_ticks - 1,
        .ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
        .RepetitionCounter = 0
    });
    LL_TIM_EnableCounter(def->reg);
    LL_TIM_EnableARRPreload(def->reg);
}

int timer_configure(
    if_pwm_t *iface,
    const if_pwm_config_t *config)
{
    timer_if_t *if_pwm = (timer_if_t *) iface;
    if_rs_t *rscs = if_pwm->header.header.rscs;
    int i;

    if (if_pwm->timer->clock_hz == 0) {
        timer_configure_main_block(if_pwm->timer, &if_pwm->def, config);
    }

    if (if_pwm->timer->clock_hz != config->clock_hz || if_pwm->timer->period_ticks != config->period_ticks) {
        return -1;
    }

    if_pwm->pulses_count = config->pulses_count;
    if_pwm->update_values_cb = config->update_values_cb;
    if_pwm->storage = config->storage;

    if_pwm->dma = dma_get(rscs[TIMER_ARG_DMA].decl->ref);
    if (if_pwm->dma == NULL) {
        return -1;
    }

    if_pwm->pulses = pvPortMalloc(sizeof(uint32_t) * config->pulses_count);
    if (if_pwm->pulses == NULL) {
        return -1;
    }

    for (i = 0; i < if_pwm->pulses_count; i++) {
        if_pwm->pulses[i] = 0;
    }

    bool chxn = (rscs[TIMER_ARG_PIN].inst->attr & 0x0100) != 0;

    LL_TIM_OC_Init(if_pwm->def.reg, if_pwm->def.channel, &(LL_TIM_OC_InitTypeDef) {
        .OCMode = LL_TIM_OCMODE_PWM1,
        .CompareValue = 0,
        .OCState = chxn ? LL_TIM_OCSTATE_DISABLE : LL_TIM_OCSTATE_ENABLE,
        .OCPolarity = LL_TIM_OCPOLARITY_HIGH,
        .OCIdleState = LL_TIM_OCIDLESTATE_LOW,
        .OCNState = chxn ? LL_TIM_OCSTATE_ENABLE : LL_TIM_OCSTATE_DISABLE,
        .OCNPolarity = LL_TIM_OCPOLARITY_HIGH,
        .OCNIdleState = LL_TIM_OCIDLESTATE_LOW
    });

    gpio_config_by_id(rscs[TIMER_ARG_PIN].decl->ref, &(LL_GPIO_InitTypeDef) {
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = rscs[TIMER_ARG_PIN].inst->attr & 0x000f
    });

    LL_DMA_Init(if_pwm->dma->reg, if_pwm->dma->stream, &(LL_DMA_InitTypeDef) {
        .PeriphOrM2MSrcAddress = if_pwm->def.compare_reg,
        .MemoryOrM2MDstAddress = (uint32_t) if_pwm->pulses,
        .Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH,
        .Mode = LL_DMA_MODE_CIRCULAR,
        .PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT,
        .MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT,
        .PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD,
        .MemoryOrM2MDstDataSize = LL_DMA_PDATAALIGN_WORD,
        .NbData = if_pwm->pulses_count,
        .Channel = rscs[TIMER_ARG_DMA].inst->attr << DMA_SxCR_CHSEL_Pos,
        .Priority = LL_DMA_PRIORITY_HIGH,
        .FIFOMode = LL_DMA_FIFOMODE_DISABLE,
        .FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_2,
        .MemBurst = LL_DMA_MBURST_SINGLE,
        .PeriphBurst = LL_DMA_PBURST_SINGLE
    });

    dma_enable_irq(if_pwm->dma, TIMER_DMA_IRQ_PRIORITY, if_pwm);
    dma_set_transfer_complete_cb(if_pwm->dma, timer_update_values_cb);

    LL_DMA_EnableStream(if_pwm->dma->reg, if_pwm->dma->stream);

    switch (if_pwm->def.channel) {
    case LL_TIM_CHANNEL_CH1:
        LL_TIM_EnableDMAReq_CC1(if_pwm->def.reg);
        break;

    case LL_TIM_CHANNEL_CH2:
        LL_TIM_EnableDMAReq_CC2(if_pwm->def.reg);
        break;

    case LL_TIM_CHANNEL_CH3:
        LL_TIM_EnableDMAReq_CC3(if_pwm->def.reg);
        break;

    case LL_TIM_CHANNEL_CH4:
        LL_TIM_EnableDMAReq_CC4(if_pwm->def.reg);
        break;
    }

    return 0;
}

static void timer_update_values_cb(
    const dma_stream_def_t *def,
    void *storage)
{
    timer_if_t *if_pwm = storage;
    if_pwm->update_values_cb(if_pwm->pulses, if_pwm->storage);
}
