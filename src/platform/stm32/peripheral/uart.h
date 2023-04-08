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

#pragma once

#include "core/peripheral.h"

#include "stm32.h"

PP_TYPE_EXTERN(uart);

typedef struct uart_def_s uart_def_t;

struct uart_def_s {
    USART_TypeDef *reg;
    uint16_t id;
    int16_t IRQn;
};

#define UART_DEF(_UART, _ID) \
    (void *) &(const uart_def_t) { \
        .reg = _UART, \
        .id = _ID, \
        .IRQn = _UART ## _IRQn \
    }

enum {
    UART_ARG_PIN_TX = 0,
    UART_ARG_PIN_RX,
    UART_ARG_DMA_TX,
    UART_ARG_DMA_RX,
    UART_NUM_ARGS
};
