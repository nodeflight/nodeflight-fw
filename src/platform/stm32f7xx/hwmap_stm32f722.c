#include "hwacc/peripheral.h"
#include "hwacc/resource.h"

#include "platform/stm32/hwacc/uart.h"

#include "stm32.h"

#include <stddef.h>

#define STRUCT(_STRUCT, ...) & (_STRUCT) {__VA_ARGS__}

PERIPHERAL_INSTANCE_DECL(uart, uart1_a09_a10, "pin_a09 pin_a10", STRUCT(uart_hwconf_t, USART1, 7, 7));
PERIPHERAL_INSTANCE_DECL(uart, uart1_b06_b07, "pin_b06 pin_b07", STRUCT(uart_hwconf_t, USART1, 7, 7));
PERIPHERAL_INSTANCE_DECL(uart, uart2_a02_a03, "pin_a02 pin_a03", STRUCT(uart_hwconf_t, USART2, 7, 7));
PERIPHERAL_INSTANCE_DECL(uart, uart2_d05_d06, "pin_d05 pin_d06", STRUCT(uart_hwconf_t, USART2, 7, 7));
PERIPHERAL_INSTANCE_DECL(uart, uart3_b10_b11, "pin_b10 pin_b11", STRUCT(uart_hwconf_t, USART3, 7, 7));
PERIPHERAL_INSTANCE_DECL(uart, uart3_c10_c11, "pin_c10 pin_c11", STRUCT(uart_hwconf_t, USART3, 7, 7));
PERIPHERAL_INSTANCE_DECL(uart, uart3_d08_d09, "pin_d08 pin_d09", STRUCT(uart_hwconf_t, USART3, 7, 7));
PERIPHERAL_INSTANCE_DECL(uart, uart4_a00_a01, "pin_a00 pin_a01", STRUCT(uart_hwconf_t, UART4, 8, 8));
PERIPHERAL_INSTANCE_DECL(uart, uart4_c10_c11, "pin_c10 pin_c11", STRUCT(uart_hwconf_t, UART4, 8, 8));
PERIPHERAL_INSTANCE_DECL(uart, uart4_h13_h14, "pin_h13 pin_h14", STRUCT(uart_hwconf_t, UART4, 8, 8));
//PERIPHERAL_INSTANCE_DECL(uart, uart4_xxx_i09, "xxx pin_i09", STRUCT(uart_hwconf_t, UART4, 8, 8));
PERIPHERAL_INSTANCE_DECL(uart, uart5_c12_d02, "pin_c12 pin_d02", STRUCT(uart_hwconf_t, UART5, 8, 8));
PERIPHERAL_INSTANCE_DECL(uart, uart6_c06_c07, "pin_c06 pin_c07", STRUCT(uart_hwconf_t, USART6, 8, 8));
PERIPHERAL_INSTANCE_DECL(uart, uart6_g14_g09, "pin_g14 pin_g09", STRUCT(uart_hwconf_t, USART6, 8, 8));
PERIPHERAL_INSTANCE_DECL(uart, uart7_e08_e07, "pin_e08 pin_e07", STRUCT(uart_hwconf_t, UART7, 8, 8));
PERIPHERAL_INSTANCE_DECL(uart, uart7_f07_f06, "pin_f07 pin_f06", STRUCT(uart_hwconf_t, UART7, 8, 8));
PERIPHERAL_INSTANCE_DECL(uart, uart8_e01_e00, "pin_e01 pin_e00", STRUCT(uart_hwconf_t, UART8, 8, 8));
