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
#include "FreeRTOS.h"

#include "platform/stm32/hwacc/gpio.h"

#include "platform/platform.h"

static void platform_clock_config(
    void);

void platform_init(
    void)
{
    /* Don't use priority grouping... */
    NVIC_SetPriorityGrouping(0);

    /* Enable all peripheral clocks. Current consumption can be optimized later */
    RCC->AHB1ENR = 0xffffffff;
    RCC->AHB2ENR = 0xffffffff;
    RCC->AHB3ENR = 0xffffffff;
    RCC->APB1ENR = 0xffffffff;
    RCC->APB2ENR = 0xffffffff;
    __DSB();

    platform_clock_config();
}

static void platform_clock_config(
    void)
{
    ErrorStatus status;
    /*
     * Configure system clock as:
     *
     * HSE - user clock -> 8MHz on nucleo f722ze
     * /M - 8 -> 1MHz
     * *N - 432 -> 432MHz
     * /P - 2 -> 216MHz
     *
     * AHB Prescaler - 1 ->216MHz
     */

    status = LL_PLL_ConfigSystemClock_HSE(8000000, LL_UTILS_HSEBYPASS_ON, &(LL_UTILS_PLLInitTypeDef) {
        .PLLM = LL_RCC_PLLM_DIV_8,
        .PLLN = 432,
        .PLLP = LL_RCC_PLLP_DIV_2
    }, &(LL_UTILS_ClkInitTypeDef) {
        .AHBCLKDivider = LL_RCC_SYSCLK_DIV_1,
        .APB1CLKDivider = LL_RCC_APB1_DIV_4,
        .APB1CLKDivider = LL_RCC_APB2_DIV_2
    });

    if(status != SUCCESS) {
        asm("bkpt 255");
    }
    LL_PWR_EnableOverDriveMode();

    /* Setup clock sources for all UARTs/USARTs */
    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2);
    LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
    LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);
    LL_RCC_SetUSARTClockSource(LL_RCC_USART6_CLKSOURCE_PCLK2);
    
    LL_RCC_SetUARTClockSource(LL_RCC_UART4_CLKSOURCE_PCLK1);
    LL_RCC_SetUARTClockSource(LL_RCC_UART5_CLKSOURCE_PCLK1);
    LL_RCC_SetUARTClockSource(LL_RCC_UART7_CLKSOURCE_PCLK1);
    LL_RCC_SetUARTClockSource(LL_RCC_UART8_CLKSOURCE_PCLK1);
}