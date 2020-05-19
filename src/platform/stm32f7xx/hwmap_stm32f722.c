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

#include "core/peripheral.h"
#include "core/resource.h"

#include "platform/stm32/hwacc/uart.h"
#include "platform/stm32/hwacc/usb_vcp.h"
#include "platform/stm32/hwacc/timer.h"

#include "stm32.h"

#include <stddef.h>

PP_INST_DECL(uart, uart1, UART_DEF(USART1, 0),
    PP_INST_RS(UART_ARG_PIN_TX, pin_none, 0),
    PP_INST_RS(UART_ARG_PIN_RX, pin_none, 0),
    PP_INST_RS(UART_ARG_DMA_TX, dma_none, 0),
    PP_INST_RS(UART_ARG_DMA_RX, dma_none, 0),

    PP_INST_RS(UART_ARG_PIN_TX, pin_a09, 7),
    PP_INST_RS(UART_ARG_PIN_TX, pin_b06, 7),

    PP_INST_RS(UART_ARG_PIN_RX, pin_a10, 7),
    PP_INST_RS(UART_ARG_PIN_RX, pin_b06, 7),

    PP_INST_RS(UART_ARG_DMA_TX, dma_2_7, 4),

    PP_INST_RS(UART_ARG_DMA_RX, dma_2_2, 4),
    PP_INST_RS(UART_ARG_DMA_RX, dma_2_5, 4)
);

PP_INST_DECL(uart, uart2, UART_DEF(USART2, 1),
    PP_INST_RS(UART_ARG_PIN_TX, pin_none, 0),
    PP_INST_RS(UART_ARG_PIN_RX, pin_none, 0),
    PP_INST_RS(UART_ARG_DMA_TX, dma_none, 0),
    PP_INST_RS(UART_ARG_DMA_RX, dma_none, 0),

    PP_INST_RS(UART_ARG_PIN_TX, pin_a02, 7),
    PP_INST_RS(UART_ARG_PIN_TX, pin_d05, 7),

    PP_INST_RS(UART_ARG_PIN_RX, pin_a03, 7),
    PP_INST_RS(UART_ARG_PIN_RX, pin_d06, 7),

    PP_INST_RS(UART_ARG_DMA_TX, dma_1_6, 4),

    PP_INST_RS(UART_ARG_DMA_RX, dma_1_5, 4)
);

PP_INST_DECL(uart, uart3, UART_DEF(USART3, 2),
    PP_INST_RS(UART_ARG_PIN_TX, pin_none, 0),
    PP_INST_RS(UART_ARG_PIN_RX, pin_none, 0),
    PP_INST_RS(UART_ARG_DMA_TX, dma_none, 0),
    PP_INST_RS(UART_ARG_DMA_RX, dma_none, 0),

    PP_INST_RS(UART_ARG_PIN_TX, pin_b10, 7),
    PP_INST_RS(UART_ARG_PIN_TX, pin_c10, 7),
    PP_INST_RS(UART_ARG_PIN_TX, pin_d08, 7),

    PP_INST_RS(UART_ARG_PIN_RX, pin_b11, 7),
    PP_INST_RS(UART_ARG_PIN_RX, pin_c11, 7),
    PP_INST_RS(UART_ARG_PIN_RX, pin_d09, 7),

    PP_INST_RS(UART_ARG_DMA_TX, dma_1_3, 4),
    PP_INST_RS(UART_ARG_DMA_TX, dma_1_4, 7),

    PP_INST_RS(UART_ARG_DMA_RX, dma_1_1, 4)
);

PP_INST_DECL(uart, uart4, UART_DEF(UART4, 3),
    PP_INST_RS(UART_ARG_PIN_TX, pin_none, 0),
    PP_INST_RS(UART_ARG_PIN_RX, pin_none, 0),
    PP_INST_RS(UART_ARG_DMA_TX, dma_none, 0),
    PP_INST_RS(UART_ARG_DMA_RX, dma_none, 0),

    PP_INST_RS(UART_ARG_PIN_TX, pin_a00, 8),
    PP_INST_RS(UART_ARG_PIN_TX, pin_c10, 8),
    PP_INST_RS(UART_ARG_PIN_TX, pin_h13, 8),

    PP_INST_RS(UART_ARG_PIN_RX, pin_a01, 8),
    PP_INST_RS(UART_ARG_PIN_RX, pin_c11, 8),
    PP_INST_RS(UART_ARG_PIN_RX, pin_h14, 8),
    PP_INST_RS(UART_ARG_PIN_RX, pin_i09, 8),

    PP_INST_RS(UART_ARG_DMA_TX, dma_1_4, 4),

    PP_INST_RS(UART_ARG_DMA_RX, dma_1_2, 4)
);

PP_INST_DECL(uart, uart5, UART_DEF(UART5, 4),
    PP_INST_RS(UART_ARG_PIN_TX, pin_none, 0),
    PP_INST_RS(UART_ARG_PIN_RX, pin_none, 0),
    PP_INST_RS(UART_ARG_DMA_TX, dma_none, 0),
    PP_INST_RS(UART_ARG_DMA_RX, dma_none, 0),

    PP_INST_RS(UART_ARG_PIN_TX, pin_c12, 8),

    PP_INST_RS(UART_ARG_PIN_RX, pin_d02, 8),

    PP_INST_RS(UART_ARG_DMA_TX, dma_1_7, 4),

    PP_INST_RS(UART_ARG_DMA_RX, dma_1_0, 4)
);

PP_INST_DECL(uart, uart6, UART_DEF(USART6, 5),
    PP_INST_RS(UART_ARG_PIN_TX, pin_none, 0),
    PP_INST_RS(UART_ARG_PIN_RX, pin_none, 0),
    PP_INST_RS(UART_ARG_DMA_TX, dma_none, 0),
    PP_INST_RS(UART_ARG_DMA_RX, dma_none, 0),

    PP_INST_RS(UART_ARG_PIN_TX, pin_c06, 8),
    PP_INST_RS(UART_ARG_PIN_TX, pin_g14, 8),

    PP_INST_RS(UART_ARG_PIN_RX, pin_c07, 8),
    PP_INST_RS(UART_ARG_PIN_RX, pin_g09, 8),

    PP_INST_RS(UART_ARG_DMA_TX, dma_2_6, 5),
    PP_INST_RS(UART_ARG_DMA_TX, dma_2_7, 5),

    PP_INST_RS(UART_ARG_DMA_RX, dma_2_1, 5),
    PP_INST_RS(UART_ARG_DMA_RX, dma_2_2, 5)
);

PP_INST_DECL(uart, uart7, UART_DEF(UART7, 6),
    PP_INST_RS(UART_ARG_PIN_TX, pin_none, 0),
    PP_INST_RS(UART_ARG_PIN_RX, pin_none, 0),
    PP_INST_RS(UART_ARG_DMA_TX, dma_none, 0),
    PP_INST_RS(UART_ARG_DMA_RX, dma_none, 0),

    PP_INST_RS(UART_ARG_PIN_TX, pin_e08, 8),
    PP_INST_RS(UART_ARG_PIN_TX, pin_f07, 8),

    PP_INST_RS(UART_ARG_PIN_RX, pin_e07, 8),
    PP_INST_RS(UART_ARG_PIN_RX, pin_f06, 8),

    PP_INST_RS(UART_ARG_DMA_TX, dma_1_1, 5),

    PP_INST_RS(UART_ARG_DMA_RX, dma_1_3, 5)
);

PP_INST_DECL(uart, uart8, UART_DEF(UART8, 7),
    PP_INST_RS(UART_ARG_PIN_TX, pin_none, 0),
    PP_INST_RS(UART_ARG_PIN_RX, pin_none, 0),
    PP_INST_RS(UART_ARG_DMA_TX, dma_none, 0),
    PP_INST_RS(UART_ARG_DMA_RX, dma_none, 0),

    PP_INST_RS(UART_ARG_PIN_TX, pin_e01, 8),

    PP_INST_RS(UART_ARG_PIN_RX, pin_e00, 8),

    PP_INST_RS(UART_ARG_DMA_TX, dma_1_0, 5),

    PP_INST_RS(UART_ARG_DMA_RX, dma_1_6, 5)
);

PP_INST_DECL(usb_vcp, usb_vcp, NULL,
    PP_INST_RS_TERMINATION
);

PP_INST_DECL(timer, tim5_ch1, TIMER_DEF(5, 1),
    PP_INST_RS(TIMER_ARG_PIN, pin_a00, 2)
);