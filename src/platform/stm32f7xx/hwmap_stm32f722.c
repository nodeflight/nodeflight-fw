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
/*
    PP_INST_RS(TIMER_ARG_PIN, pin_a00, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_2, 6)
*/

PP_INST_DECL(timer, tim1_ch1, TIMER_DEF(1, 1),
    PP_INST_RS(TIMER_ARG_PIN, pin_a07, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_a08, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_e08, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_e09, 0x0001),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_6, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_3, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_1, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim1_ch2, TIMER_DEF(1, 2),
    PP_INST_RS(TIMER_ARG_PIN, pin_a09, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b00, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_e10, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_e11, 0x0001),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_2, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_6, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim1_ch3, TIMER_DEF(1, 3),
    PP_INST_RS(TIMER_ARG_PIN, pin_a10, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b01, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_e12, 0x0101),
    PP_INST_RS(TIMER_ARG_PIN, pin_e13, 0x0001),
    // PP_INST_RS(TIMER_ARG_DMA, dma_2_6, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_6, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim1_ch4, TIMER_DEF(1, 4),
    PP_INST_RS(TIMER_ARG_PIN, pin_a11, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_e14, 0x0001),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_4, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim2_ch1, TIMER_DEF(2, 1),
    PP_INST_RS(TIMER_ARG_PIN, pin_a00, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_a05, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_a15, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b13, 0x0101),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_5, 3),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim2_ch2, TIMER_DEF(2, 2),
    PP_INST_RS(TIMER_ARG_PIN, pin_a01, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b03, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b14, 0x0101),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_6, 3),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim2_ch3, TIMER_DEF(2, 3),
    PP_INST_RS(TIMER_ARG_PIN, pin_a02, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b10, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b15, 0x0101),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_1, 3),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim2_ch4, TIMER_DEF(2, 4),
    PP_INST_RS(TIMER_ARG_PIN, pin_a03, 0x0001),
    PP_INST_RS(TIMER_ARG_PIN, pin_b11, 0x0001),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_6, 3),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_7, 3),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim3_ch1, TIMER_DEF(3, 1),
    PP_INST_RS(TIMER_ARG_PIN, pin_a06, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_b04, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_c06, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_4, 5),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim3_ch2, TIMER_DEF(3, 2),
    PP_INST_RS(TIMER_ARG_PIN, pin_a07, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_b05, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_c07, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_5, 5),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim3_ch3, TIMER_DEF(3, 3),
    PP_INST_RS(TIMER_ARG_PIN, pin_b00, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_c08, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_7, 5),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim3_ch4, TIMER_DEF(3, 4),
    PP_INST_RS(TIMER_ARG_PIN, pin_b01, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_c09, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_2, 5),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim4_ch1, TIMER_DEF(4, 1),
    PP_INST_RS(TIMER_ARG_PIN, pin_b06, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_0, 2),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim4_ch2, TIMER_DEF(4, 2),
    PP_INST_RS(TIMER_ARG_PIN, pin_b07, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_2, 2),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim4_ch3, TIMER_DEF(4, 3),
    PP_INST_RS(TIMER_ARG_PIN, pin_b08, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_7, 2),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim4_ch4, TIMER_DEF(4, 4),
    PP_INST_RS(TIMER_ARG_PIN, pin_b09, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim5_ch1, TIMER_DEF(5, 1),
    PP_INST_RS(TIMER_ARG_PIN, pin_a00, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_d12, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_h10, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_2, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim5_ch2, TIMER_DEF(5, 2),
    PP_INST_RS(TIMER_ARG_PIN, pin_a01, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_d13, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_h11, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_4, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim5_ch3, TIMER_DEF(5, 3),
    PP_INST_RS(TIMER_ARG_PIN, pin_a02, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_d14, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_h12, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_0, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim5_ch4, TIMER_DEF(5, 4),
    PP_INST_RS(TIMER_ARG_PIN, pin_a03, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_d15, 0x0002),
    PP_INST_RS(TIMER_ARG_PIN, pin_i00, 0x0002),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_1, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_1_3, 6),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

// PP_INST_DECL(timer, tim5_ch1, TIMER_DEF(5, 1),
// );
// PP_INST_DECL(timer, tim5_ch2, TIMER_DEF(5, 2),
// );
// PP_INST_DECL(timer, tim5_ch3, TIMER_DEF(5, 3),
// );
// PP_INST_DECL(timer, tim5_ch4, TIMER_DEF(5, 4),
// );

// PP_INST_DECL(timer, tim6_ch1, TIMER_DEF(6, 1),
// );
// PP_INST_DECL(timer, tim6_ch2, TIMER_DEF(6, 2),
// );
// PP_INST_DECL(timer, tim6_ch3, TIMER_DEF(6, 3),
// );
// PP_INST_DECL(timer, tim6_ch4, TIMER_DEF(6, 4),
// );

// PP_INST_DECL(timer, tim7_ch1, TIMER_DEF(7, 1),
// );
// PP_INST_DECL(timer, tim7_ch2, TIMER_DEF(7, 2),
// );
// PP_INST_DECL(timer, tim7_ch3, TIMER_DEF(7, 3),
// );
// PP_INST_DECL(timer, tim7_ch4, TIMER_DEF(7, 4),
// );

PP_INST_DECL(timer, tim8_ch1, TIMER_DEF(8, 1),
    PP_INST_RS(TIMER_ARG_PIN, pin_a05, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_a07, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_c06, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_h13, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_i05, 0x0003),
    // PP_INST_RS(TIMER_ARG_DMA, dma_2_2, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_2, 7),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim8_ch2, TIMER_DEF(8, 2),
    PP_INST_RS(TIMER_ARG_PIN, pin_b00, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_b14, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_c07, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_h14, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_i06, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_2, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_3, 7),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim8_ch3, TIMER_DEF(8, 3),
    PP_INST_RS(TIMER_ARG_PIN, pin_b01, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_b15, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_c08, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_h15, 0x0103),
    PP_INST_RS(TIMER_ARG_PIN, pin_i07, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_2, 0),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_4, 7),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim8_ch4, TIMER_DEF(8, 4),
    PP_INST_RS(TIMER_ARG_PIN, pin_c09, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_i02, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_2_7, 7),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);

PP_INST_DECL(timer, tim9_ch1, TIMER_DEF(9, 1),
    PP_INST_RS(TIMER_ARG_PIN, pin_a02, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_e05, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
PP_INST_DECL(timer, tim9_ch2, TIMER_DEF(9, 2),
    PP_INST_RS(TIMER_ARG_PIN, pin_a03, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_e06, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim9_ch3, TIMER_DEF(9, 3),
// );
// PP_INST_DECL(timer, tim9_ch4, TIMER_DEF(9, 4),
// );

PP_INST_DECL(timer, tim10_ch1, TIMER_DEF(10, 1),
    PP_INST_RS(TIMER_ARG_PIN, pin_b08, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_f06, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim10_ch2, TIMER_DEF(10, 2),
// );
// PP_INST_DECL(timer, tim10_ch3, TIMER_DEF(10, 3),
// );
// PP_INST_DECL(timer, tim10_ch4, TIMER_DEF(10, 4),
// );

PP_INST_DECL(timer, tim11_ch1, TIMER_DEF(11, 1),
    PP_INST_RS(TIMER_ARG_PIN, pin_b09, 0x0003),
    PP_INST_RS(TIMER_ARG_PIN, pin_f07, 0x0003),
    PP_INST_RS(TIMER_ARG_DMA, dma_none, 0)
);
// PP_INST_DECL(timer, tim11_ch2, TIMER_DEF(11, 2),
// );
// PP_INST_DECL(timer, tim11_ch3, TIMER_DEF(11, 3),
// );
// PP_INST_DECL(timer, tim11_ch4, TIMER_DEF(11, 4),
// );

// PP_INST_DECL(timer, tim12_ch1, TIMER_DEF(12, 1),
// );
// PP_INST_DECL(timer, tim12_ch2, TIMER_DEF(12, 2),
// );
// PP_INST_DECL(timer, tim12_ch3, TIMER_DEF(12, 3),
// );
// PP_INST_DECL(timer, tim12_ch4, TIMER_DEF(12, 4),
// );

// PP_INST_DECL(timer, tim13_ch1, TIMER_DEF(13, 1),
// );
// PP_INST_DECL(timer, tim13_ch2, TIMER_DEF(13, 2),
// );
// PP_INST_DECL(timer, tim13_ch3, TIMER_DEF(13, 3),
// );
// PP_INST_DECL(timer, tim13_ch4, TIMER_DEF(13, 4),
// );
