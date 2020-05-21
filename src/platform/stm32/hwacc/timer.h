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

#pragma once

#include "core/peripheral.h"

#include "stm32.h"

PP_TYPE_EXTERN(timer);

typedef struct timer_def_s timer_def_t;

struct timer_def_s {
    TIM_TypeDef *reg;
    uint32_t compare_reg;
    uint32_t channel;
    uint8_t timer_id; /* TIM resource id enumerated from 0 (TIM1 = id 0, TIM3 = id 2) */
};

#define TIMER_DEF(_TIMER_ID, _CH) \
    (void *) &(const timer_def_t) { \
        .reg = TIM ## _TIMER_ID, \
        .compare_reg = (uint32_t) &TIM ## _TIMER_ID->CCR ## _CH, \
        .channel = LL_TIM_CHANNEL_CH ## _CH, \
        .timer_id = _TIMER_ID - 1 \
    }

enum {
    TIMER_ARG_PIN = 0,
    TIMER_ARG_DMA,
    TIMER_NUM_ARGS
};
