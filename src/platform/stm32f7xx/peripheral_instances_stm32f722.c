#include "peripheral/peripheral.h"

#include "platform/stm32/peripheral/uart.h"

#include <stddef.h>

PERIPHERAL_INSTANCE_DECL(uart, uart1_a09_a10, "UART1 @ A09 A10", NULL);
PERIPHERAL_INSTANCE_DECL(uart, uart1_b06_b07, "UART1 @ B06 B07", NULL);
PERIPHERAL_INSTANCE_DECL(uart, uart2_a02_a03, "UART2 @ A02 A03", NULL);
PERIPHERAL_INSTANCE_DECL(uart, uart2_d05_d06, "UART2 @ D05 D06", NULL);
PERIPHERAL_INSTANCE_DECL(uart, uart3_b10_b11, "UART3 @ B10 B11", NULL);
PERIPHERAL_INSTANCE_DECL(uart, uart3_c10_c11, "UART3 @ C10 C11", NULL);
PERIPHERAL_INSTANCE_DECL(uart, uart3_d08_d09, "UART3 @ D08 D09", NULL);
PERIPHERAL_INSTANCE_DECL(uart, uart4_c10_c11, "UART4 @ C10 C11", NULL);
PERIPHERAL_INSTANCE_DECL(uart, uart6_c06_c07, "UART6 @ C06 C07", NULL);
PERIPHERAL_INSTANCE_DECL(uart, uart6_g14_g09, "UART6 @ G14 G09", NULL);