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

#include "core/resource.h"

#include "stm32.h"

#define GPIO_ID(_REF) ((#_REF)[0] - 'a') << 8 | (((#_REF)[1] - '0') *10 + ((#_REF)[2] - '0'))

void gpio_config_by_id(
    uint32_t id,
    LL_GPIO_InitTypeDef *init_struct);