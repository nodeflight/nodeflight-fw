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

/* Just to be able to include the correct HAL drivers independent of platform */

#include "stm32f7xx.h"
#include "stm32f7xx_ll_dma.h"
#include "stm32f7xx_ll_exti.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_spi.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_tim.h"
#include "stm32f7xx_ll_usart.h"
#include "stm32f7xx_ll_usb.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_rng.h"

/* Maximum number of resources available, sets limit of allocated arrays for storage */
#define UART_MAX_COUNT 8
#define TIMER_MAX_COUNT 14
#define SPI_MAX_COUNT 5
