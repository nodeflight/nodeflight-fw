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

#include "stm32.h"
#include "systime.h"

static volatile uint32_t systime_high = 0;

void systime_init(
    void)
{
    LL_TIM_Init(TIM7, &(LL_TIM_InitTypeDef) {
        .Prescaler = (SystemCoreClock / 1000000) - 1, /* Prescale down to 1us */
        .CounterMode = LL_TIM_COUNTERMODE_UP,
        .Autoreload = 0xffff,
        .ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
        .RepetitionCounter = 0
    });
    LL_TIM_EnableCounter(TIM7);
    LL_TIM_EnableIT_UPDATE(TIM7);
    NVIC_SetPriority(TIM7_IRQn, 0); /* The update needs to be atomic, prio 0 */
    NVIC_EnableIRQ(TIM7_IRQn);
}

uint32_t systime_get_runtime(
    void)
{
    volatile uint32_t high_bits;
    volatile uint32_t low_bits;

    /* if overflow while reading, the high bits should have changed. Then retry */
    do {
        high_bits = systime_high;
        low_bits = LL_TIM_GetCounter(TIM7);
    } while(systime_high != high_bits);

    return low_bits | high_bits;
}

void TIM7_IRQHandler(
    void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM7)) {
        LL_TIM_ClearFlag_UPDATE(TIM7);
        systime_high += 0x00010000;
    }
}
