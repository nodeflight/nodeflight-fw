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

#include "platform/stm32/peripheral/gpio.h"
#include "platform/stm32/peripheral/spi.h"
#include "platform/stm32/peripheral/uart.h"
#include "platform/stm32/peripheral/usb_vcp.h"
#include "platform/stm32/peripheral/timer.h"

#include "stm32.h"

#include <stddef.h>

PP_INST_DECL(gpio, gpio_a00, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a00, 0));
PP_INST_DECL(gpio, gpio_a01, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a01, 0));
PP_INST_DECL(gpio, gpio_a02, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a02, 0));
PP_INST_DECL(gpio, gpio_a03, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a03, 0));
PP_INST_DECL(gpio, gpio_a04, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a04, 0));
PP_INST_DECL(gpio, gpio_a05, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a05, 0));
PP_INST_DECL(gpio, gpio_a06, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a06, 0));
PP_INST_DECL(gpio, gpio_a07, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a07, 0));
PP_INST_DECL(gpio, gpio_a08, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a08, 0));
PP_INST_DECL(gpio, gpio_a09, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a09, 0));
PP_INST_DECL(gpio, gpio_a10, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a10, 0));
PP_INST_DECL(gpio, gpio_a11, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a11, 0));
PP_INST_DECL(gpio, gpio_a12, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a12, 0));
PP_INST_DECL(gpio, gpio_a13, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a13, 0));
PP_INST_DECL(gpio, gpio_a14, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a14, 0));
PP_INST_DECL(gpio, gpio_a15, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_a15, 0));
PP_INST_DECL(gpio, gpio_b00, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b00, 0));
PP_INST_DECL(gpio, gpio_b01, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b01, 0));
PP_INST_DECL(gpio, gpio_b02, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b02, 0));
PP_INST_DECL(gpio, gpio_b03, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b03, 0));
PP_INST_DECL(gpio, gpio_b04, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b04, 0));
PP_INST_DECL(gpio, gpio_b05, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b05, 0));
PP_INST_DECL(gpio, gpio_b06, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b06, 0));
PP_INST_DECL(gpio, gpio_b07, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b07, 0));
PP_INST_DECL(gpio, gpio_b08, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b08, 0));
PP_INST_DECL(gpio, gpio_b09, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b09, 0));
PP_INST_DECL(gpio, gpio_b10, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b10, 0));
PP_INST_DECL(gpio, gpio_b11, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b11, 0));
PP_INST_DECL(gpio, gpio_b12, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b12, 0));
PP_INST_DECL(gpio, gpio_b13, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b13, 0));
PP_INST_DECL(gpio, gpio_b14, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b14, 0));
PP_INST_DECL(gpio, gpio_b15, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_b15, 0));

PP_INST_DECL(gpio, gpio_c00, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c00, 0));
PP_INST_DECL(gpio, gpio_c01, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c01, 0));
PP_INST_DECL(gpio, gpio_c02, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c02, 0));
PP_INST_DECL(gpio, gpio_c03, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c03, 0));
PP_INST_DECL(gpio, gpio_c04, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c04, 0));
PP_INST_DECL(gpio, gpio_c05, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c05, 0));
PP_INST_DECL(gpio, gpio_c06, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c06, 0));
PP_INST_DECL(gpio, gpio_c07, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c07, 0));
PP_INST_DECL(gpio, gpio_c08, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c08, 0));
PP_INST_DECL(gpio, gpio_c09, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c09, 0));
PP_INST_DECL(gpio, gpio_c10, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c10, 0));
PP_INST_DECL(gpio, gpio_c11, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c11, 0));
PP_INST_DECL(gpio, gpio_c12, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c12, 0));
PP_INST_DECL(gpio, gpio_c13, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c13, 0));
PP_INST_DECL(gpio, gpio_c14, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c14, 0));
PP_INST_DECL(gpio, gpio_c15, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_c15, 0));

PP_INST_DECL(gpio, gpio_d00, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d00, 0));
PP_INST_DECL(gpio, gpio_d01, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d01, 0));
PP_INST_DECL(gpio, gpio_d02, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d02, 0));
PP_INST_DECL(gpio, gpio_d03, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d03, 0));
PP_INST_DECL(gpio, gpio_d04, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d04, 0));
PP_INST_DECL(gpio, gpio_d05, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d05, 0));
PP_INST_DECL(gpio, gpio_d06, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d06, 0));
PP_INST_DECL(gpio, gpio_d07, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d07, 0));
PP_INST_DECL(gpio, gpio_d08, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d08, 0));
PP_INST_DECL(gpio, gpio_d09, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d09, 0));
PP_INST_DECL(gpio, gpio_d10, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d10, 0));
PP_INST_DECL(gpio, gpio_d11, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d11, 0));
PP_INST_DECL(gpio, gpio_d12, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d12, 0));
PP_INST_DECL(gpio, gpio_d13, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d13, 0));
PP_INST_DECL(gpio, gpio_d14, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d14, 0));
PP_INST_DECL(gpio, gpio_d15, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_d15, 0));

PP_INST_DECL(gpio, gpio_e00, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e00, 0));
PP_INST_DECL(gpio, gpio_e01, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e01, 0));
PP_INST_DECL(gpio, gpio_e02, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e02, 0));
PP_INST_DECL(gpio, gpio_e03, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e03, 0));
PP_INST_DECL(gpio, gpio_e04, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e04, 0));
PP_INST_DECL(gpio, gpio_e05, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e05, 0));
PP_INST_DECL(gpio, gpio_e06, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e06, 0));
PP_INST_DECL(gpio, gpio_e07, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e07, 0));
PP_INST_DECL(gpio, gpio_e08, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e08, 0));
PP_INST_DECL(gpio, gpio_e09, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e09, 0));
PP_INST_DECL(gpio, gpio_e10, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e10, 0));
PP_INST_DECL(gpio, gpio_e11, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e11, 0));
PP_INST_DECL(gpio, gpio_e12, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e12, 0));
PP_INST_DECL(gpio, gpio_e13, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e13, 0));
PP_INST_DECL(gpio, gpio_e14, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e14, 0));
PP_INST_DECL(gpio, gpio_e15, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_e15, 0));

PP_INST_DECL(gpio, gpio_f00, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f00, 0));
PP_INST_DECL(gpio, gpio_f01, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f01, 0));
PP_INST_DECL(gpio, gpio_f02, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f02, 0));
PP_INST_DECL(gpio, gpio_f03, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f03, 0));
PP_INST_DECL(gpio, gpio_f04, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f04, 0));
PP_INST_DECL(gpio, gpio_f05, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f05, 0));
PP_INST_DECL(gpio, gpio_f06, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f06, 0));
PP_INST_DECL(gpio, gpio_f07, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f07, 0));
PP_INST_DECL(gpio, gpio_f08, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f08, 0));
PP_INST_DECL(gpio, gpio_f09, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f09, 0));
PP_INST_DECL(gpio, gpio_f10, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f10, 0));
PP_INST_DECL(gpio, gpio_f11, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f11, 0));
PP_INST_DECL(gpio, gpio_f12, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f12, 0));
PP_INST_DECL(gpio, gpio_f13, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f13, 0));
PP_INST_DECL(gpio, gpio_f14, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f14, 0));
PP_INST_DECL(gpio, gpio_f15, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_f15, 0));

PP_INST_DECL(gpio, gpio_g00, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g00, 0));
PP_INST_DECL(gpio, gpio_g01, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g01, 0));
PP_INST_DECL(gpio, gpio_g02, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g02, 0));
PP_INST_DECL(gpio, gpio_g03, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g03, 0));
PP_INST_DECL(gpio, gpio_g04, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g04, 0));
PP_INST_DECL(gpio, gpio_g05, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g05, 0));
PP_INST_DECL(gpio, gpio_g06, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g06, 0));
PP_INST_DECL(gpio, gpio_g07, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g07, 0));
PP_INST_DECL(gpio, gpio_g08, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g08, 0));
PP_INST_DECL(gpio, gpio_g09, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g09, 0));
PP_INST_DECL(gpio, gpio_g10, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g10, 0));
PP_INST_DECL(gpio, gpio_g11, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g11, 0));
PP_INST_DECL(gpio, gpio_g12, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g12, 0));
PP_INST_DECL(gpio, gpio_g13, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g13, 0));
PP_INST_DECL(gpio, gpio_g14, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g14, 0));
PP_INST_DECL(gpio, gpio_g15, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_g15, 0));

PP_INST_DECL(gpio, gpio_h00, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h00, 0));
PP_INST_DECL(gpio, gpio_h01, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h01, 0));
PP_INST_DECL(gpio, gpio_h02, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h02, 0));
PP_INST_DECL(gpio, gpio_h03, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h03, 0));
PP_INST_DECL(gpio, gpio_h04, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h04, 0));
PP_INST_DECL(gpio, gpio_h05, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h05, 0));
PP_INST_DECL(gpio, gpio_h06, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h06, 0));
PP_INST_DECL(gpio, gpio_h07, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h07, 0));
PP_INST_DECL(gpio, gpio_h08, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h08, 0));
PP_INST_DECL(gpio, gpio_h09, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h09, 0));
PP_INST_DECL(gpio, gpio_h10, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h10, 0));
PP_INST_DECL(gpio, gpio_h11, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h11, 0));
PP_INST_DECL(gpio, gpio_h12, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h12, 0));
PP_INST_DECL(gpio, gpio_h13, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h13, 0));
PP_INST_DECL(gpio, gpio_h14, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h14, 0));
PP_INST_DECL(gpio, gpio_h15, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_h15, 0));

PP_INST_DECL(gpio, gpio_i00, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i00, 0));
PP_INST_DECL(gpio, gpio_i01, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i01, 0));
PP_INST_DECL(gpio, gpio_i02, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i02, 0));
PP_INST_DECL(gpio, gpio_i03, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i03, 0));
PP_INST_DECL(gpio, gpio_i04, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i04, 0));
PP_INST_DECL(gpio, gpio_i05, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i05, 0));
PP_INST_DECL(gpio, gpio_i06, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i06, 0));
PP_INST_DECL(gpio, gpio_i07, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i07, 0));
PP_INST_DECL(gpio, gpio_i08, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i08, 0));
PP_INST_DECL(gpio, gpio_i09, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i09, 0));
PP_INST_DECL(gpio, gpio_i10, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i10, 0));
PP_INST_DECL(gpio, gpio_i11, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i11, 0));
PP_INST_DECL(gpio, gpio_i12, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i12, 0));
PP_INST_DECL(gpio, gpio_i13, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i13, 0));
PP_INST_DECL(gpio, gpio_i14, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i14, 0));
PP_INST_DECL(gpio, gpio_i15, NULL, PP_INST_RS(GPIO_ARG_PIN, pin_i15, 0));

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

/* Is missing, and never used. But needs to have a value to work with non-N channels */
#define LL_TIM_CHANNEL_CH4N 0

PP_INST_DECL(timer, tim1_ch1, TIMER_DEF(1, 1, TIM1_UP_TIM10_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a07, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_a08, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_e08, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_e09, 0x0001),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_6, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_3, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_1, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim1_ch2, TIMER_DEF(1, 2, TIM1_UP_TIM10_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a09, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b00, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_e10, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_e11, 0x0001),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_2, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_6, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim1_ch3, TIMER_DEF(1, 3, TIM1_UP_TIM10_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a10, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b01, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_e12, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_e13, 0x0001),
    // PP_INST_RS(TIMER_ARG_DMA, dma_2_6, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_6, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim1_ch4, TIMER_DEF(1, 4, TIM1_UP_TIM10_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a11, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_e14, 0x0001),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_4, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim2_ch1, TIMER_DEF(2, 1, TIM2_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a00, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_a05, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_a15, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b13, 0x0101),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_5, 3),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim2_ch2, TIMER_DEF(2, 2, TIM2_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a01, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b03, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b14, 0x0101),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_6, 3),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim2_ch3, TIMER_DEF(2, 3, TIM2_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a02, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b10, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b15, 0x0101),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_1, 3),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim2_ch4, TIMER_DEF(2, 4, TIM2_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a03, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b11, 0x0001),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_6, 3),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_7, 3),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim3_ch1, TIMER_DEF(3, 1, TIM3_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a06, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_b04, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_c06, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_4, 5),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim3_ch2, TIMER_DEF(3, 2, TIM3_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a07, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_b05, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_c07, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_5, 5),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim3_ch3, TIMER_DEF(3, 3, TIM3_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b00, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_c08, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_7, 5),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim3_ch4, TIMER_DEF(3, 4, TIM3_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b01, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_c09, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_2, 5),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim4_ch1, TIMER_DEF(4, 1, TIM4_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b06, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_0, 2),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim4_ch2, TIMER_DEF(4, 2, TIM4_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b07, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_2, 2),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim4_ch3, TIMER_DEF(4, 3, TIM4_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b08, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_7, 2),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim4_ch4, TIMER_DEF(4, 4, TIM4_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b09, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim5_ch1, TIMER_DEF(5, 1, TIM5_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a00, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_d12, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_h10, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_2, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim5_ch2, TIMER_DEF(5, 2, TIM5_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a01, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_d13, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_h11, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_4, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim5_ch3, TIMER_DEF(5, 3, TIM5_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a02, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_d14, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_h12, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_0, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim5_ch4, TIMER_DEF(5, 4, TIM5_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a03, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_d15, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_i00, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_1, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_3, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim6_ch1, TIMER_DEF(6, 1, TIM6_DAC_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim6_ch2, TIMER_DEF(6, 2, TIM6_DAC_IRQn),
// );
// PP_INST_DECL(timer, tim6_ch3, TIMER_DEF(6, 3, TIM6_DAC_IRQn),
// );
// PP_INST_DECL(timer, tim6_ch4, TIMER_DEF(6, 4, TIM6_DAC_IRQn),
// );

PP_INST_DECL(timer, tim7_ch1, TIMER_DEF(7, 1, TIM7_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim7_ch2, TIMER_DEF(7, 2, TIM7_IRQn),
// );
// PP_INST_DECL(timer, tim7_ch3, TIMER_DEF(7, 3, TIM7_IRQn),
// );
// PP_INST_DECL(timer, tim7_ch4, TIMER_DEF(7, 4, TIM7_IRQn),
// );

PP_INST_DECL(timer, tim8_ch1, TIMER_DEF(8, 1, TIM8_UP_TIM13_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a05, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_a07, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_c06, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_h13, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_i05, 0x0003),
    // PP_INST_RS(TIMER_ARG_DMA, dma_2_2, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_2, 7),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim8_ch2, TIMER_DEF(8, 2, TIM8_UP_TIM13_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b00, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_b14, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_c07, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_h14, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_i06, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_2, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_3, 7),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim8_ch3, TIMER_DEF(8, 3, TIM8_UP_TIM13_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b01, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_b15, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_c08, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_h15, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_i07, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_2, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_4, 7),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim8_ch4, TIMER_DEF(8, 4, TIM8_UP_TIM13_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_c09, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_i02, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_7, 7),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim9_ch1, TIMER_DEF(9, 1, TIM1_BRK_TIM9_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a02, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_e05, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim9_ch2, TIMER_DEF(9, 2, TIM1_BRK_TIM9_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a03, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_e06, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim9_ch3, TIMER_DEF(9, 3, TIM1_BRK_TIM9_IRQn),
// );
// PP_INST_DECL(timer, tim9_ch4, TIMER_DEF(9, 4, TIM1_BRK_TIM9_IRQn),
// );

PP_INST_DECL(timer, tim10_ch1, TIMER_DEF(10, 1, TIM1_UP_TIM10_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b08, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_f06, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim10_ch2, TIMER_DEF(10, 2, TIM1_UP_TIM10_IRQn),
// );
// PP_INST_DECL(timer, tim10_ch3, TIMER_DEF(10, 3, TIM1_UP_TIM10_IRQn),
// );
// PP_INST_DECL(timer, tim10_ch4, TIMER_DEF(10, 4, TIM1_UP_TIM10_IRQn),
// );

PP_INST_DECL(timer, tim11_ch1, TIMER_DEF(11, 1, TIM1_TRG_COM_TIM11_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b09, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_f07, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim11_ch2, TIMER_DEF(11, 2, TIM1_TRG_COM_TIM11_IRQn),
// );
// PP_INST_DECL(timer, tim11_ch3, TIMER_DEF(11, 3, TIM1_TRG_COM_TIM11_IRQn),
// );
// PP_INST_DECL(timer, tim11_ch4, TIMER_DEF(11, 4, TIM1_TRG_COM_TIM11_IRQn),
// );

PP_INST_DECL(timer, tim12_ch1, TIMER_DEF(12, 1, TIM8_BRK_TIM12_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b14, 0x0009),
    PP_INST_RS(TIMER_ARG_PIN, pin_h06, 0x0009),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim12_ch2, TIMER_DEF(12, 2, TIM8_BRK_TIM12_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_b15, 0x0009),
    PP_INST_RS(TIMER_ARG_PIN, pin_h09, 0x0009),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim12_ch3, TIMER_DEF(12, 3, TIM8_BRK_TIM12_IRQn),
// );
// PP_INST_DECL(timer, tim12_ch4, TIMER_DEF(12, 4, TIM8_BRK_TIM12_IRQn),
// );

PP_INST_DECL(timer, tim13_ch1, TIMER_DEF(13, 1, TIM8_UP_TIM13_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a06, 0x0009),
    PP_INST_RS(TIMER_ARG_PIN, pin_f08, 0x0009),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim13_ch2, TIMER_DEF(13, 2, TIM8_UP_TIM13_IRQn),
// );
// PP_INST_DECL(timer, tim13_ch3, TIMER_DEF(13, 3, TIM8_UP_TIM13_IRQn),
// );
// PP_INST_DECL(timer, tim13_ch4, TIMER_DEF(13, 4, TIM8_UP_TIM13_IRQn),
// );

PP_INST_DECL(timer, tim14_ch1, TIMER_DEF(14, 1, TIM8_TRG_COM_TIM14_IRQn),
    PP_INST_RS(TIMER_ARG_PIN, pin_none, 0),
    PP_INST_RS(TIMER_ARG_PIN, pin_a07, 0x0009),
    PP_INST_RS(TIMER_ARG_PIN, pin_f09, 0x0009),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim14_ch2, TIMER_DEF(14, 2, TIM8_TRG_COM_TIM14_IRQn),
// );
// PP_INST_DECL(timer, tim14_ch3, TIMER_DEF(14, 3, TIM8_TRG_COM_TIM14_IRQn),
// );
// PP_INST_DECL(timer, tim14_ch4, TIMER_DEF(14, 4, TIM8_TRG_COM_TIM14_IRQn),
// );

PP_INST_DECL(spi, spi1, SPI_DEF(1),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_a05, 5),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_b03, 5),

    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_a07, 5),
    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_b05, 5),

    PP_INST_RS(SPI_ARG_PIN_MISO, pin_a06, 5),
    PP_INST_RS(SPI_ARG_PIN_MISO, pin_b04, 5),

    PP_INST_RS(SPI_ARG_DMA_TX, dma_2_3, 3),
    PP_INST_RS(SPI_ARG_DMA_TX, dma_2_5, 3),
    PP_INST_RS(SPI_ARG_DMA_TX, dma_none, 0),

    PP_INST_RS(SPI_ARG_DMA_RX, dma_2_0, 3),
    PP_INST_RS(SPI_ARG_DMA_RX, dma_2_2, 3),
    PP_INST_RS(SPI_ARG_DMA_RX, dma_none, 0)
);

PP_INST_DECL(spi, spi2, SPI_DEF(2),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_a09, 5),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_b10, 5),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_b13, 5),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_d03, 5),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_i01, 5),

    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_b15, 5),
    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_c01, 5),
    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_c03, 5),
    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_i03, 5),

    PP_INST_RS(SPI_ARG_PIN_MISO, pin_b14, 5),
    PP_INST_RS(SPI_ARG_PIN_MISO, pin_c02, 5),
    PP_INST_RS(SPI_ARG_PIN_MISO, pin_i02, 5),

    PP_INST_RS(SPI_ARG_DMA_TX, dma_1_4, 0),
    PP_INST_RS(SPI_ARG_DMA_TX, dma_none, 0),

    PP_INST_RS(SPI_ARG_DMA_RX, dma_1_3, 0),
    PP_INST_RS(SPI_ARG_DMA_RX, dma_none, 0)
);

PP_INST_DECL(spi, spi3, SPI_DEF(3),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_b03, 6),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_c10, 6),

    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_b05, 6),
    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_b02, 7),
    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_c12, 6),
    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_d06, 5),

    PP_INST_RS(SPI_ARG_PIN_MISO, pin_b04, 6),
    PP_INST_RS(SPI_ARG_PIN_MISO, pin_c11, 6),

    PP_INST_RS(SPI_ARG_DMA_TX, dma_1_5, 0),
    PP_INST_RS(SPI_ARG_DMA_TX, dma_1_7, 0),
    PP_INST_RS(SPI_ARG_DMA_TX, dma_none, 0),

    PP_INST_RS(SPI_ARG_DMA_RX, dma_1_0, 0),
    PP_INST_RS(SPI_ARG_DMA_RX, dma_1_2, 0),
    PP_INST_RS(SPI_ARG_DMA_RX, dma_none, 0)
);

PP_INST_DECL(spi, spi4, SPI_DEF(4),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_e02, 5),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_e12, 5),

    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_e06, 5),
    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_e14, 5),

    PP_INST_RS(SPI_ARG_PIN_MISO, pin_e05, 5),
    PP_INST_RS(SPI_ARG_PIN_MISO, pin_e13, 5),

    PP_INST_RS(SPI_ARG_DMA_TX, dma_2_1, 4),
    PP_INST_RS(SPI_ARG_DMA_TX, dma_2_4, 5),
    PP_INST_RS(SPI_ARG_DMA_TX, dma_none, 0),

    PP_INST_RS(SPI_ARG_DMA_RX, dma_2_0, 4),
    PP_INST_RS(SPI_ARG_DMA_RX, dma_2_3, 5),
    PP_INST_RS(SPI_ARG_DMA_RX, dma_none, 0)
);

PP_INST_DECL(spi, spi5, SPI_DEF(5),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_f07, 5),
    PP_INST_RS(SPI_ARG_PIN_SCK, pin_h06, 5),

    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_f09, 5),
    PP_INST_RS(SPI_ARG_PIN_MOSI, pin_f11, 5),

    PP_INST_RS(SPI_ARG_PIN_MISO, pin_f08, 5),
    PP_INST_RS(SPI_ARG_PIN_MISO, pin_h07, 5),

    PP_INST_RS(SPI_ARG_DMA_TX, dma_2_4, 2),
    PP_INST_RS(SPI_ARG_DMA_TX, dma_2_6, 7),
    PP_INST_RS(SPI_ARG_DMA_TX, dma_none, 0),

    PP_INST_RS(SPI_ARG_DMA_RX, dma_2_3, 2),
    PP_INST_RS(SPI_ARG_DMA_RX, dma_2_5, 7),
    PP_INST_RS(SPI_ARG_DMA_RX, dma_none, 0)
);
