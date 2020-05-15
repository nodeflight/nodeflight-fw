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

#include "platform/stm32/hwacc/gpio.h"
#include "core/resource.h"

#include "stm32.h"

#include <stddef.h>

static GPIO_TypeDef *const gpio_ports[] = {
#ifdef GPIOA
    [0] = GPIOA,
#endif
#ifdef GPIOB
    [1] = GPIOB,
#endif
#ifdef GPIOC
    [2] = GPIOC,
#endif
#ifdef GPIOD
    [3] = GPIOD,
#endif
#ifdef GPIOE
    [4] = GPIOE,
#endif
#ifdef GPIOF
    [5] = GPIOF,
#endif
#ifdef GPIOG
    [6] = GPIOG,
#endif
#ifdef GPIOH
    [7] = GPIOH,
#endif
#ifdef GPIOI
    [8] = GPIOI,
#endif
#ifdef GPIOJ
    [9] = GPIOJ,
#endif
#ifdef GPIOK
    [10] = GPIOK,
#endif
};

void gpio_config_by_id(
    uint32_t id,
    LL_GPIO_InitTypeDef *init_struct)
{
    init_struct->Pin = 1 << (id & 0x000000ff);
    LL_GPIO_Init(gpio_ports[(id & 0x0000ff00) >> 8], init_struct);
}