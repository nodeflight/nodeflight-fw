#pragma once

#include "hwacc/peripheral.h"

#include "stm32.h"

typedef struct uart_hwconf_s uart_hwconf_t;

struct uart_hwconf_s {
    USART_TypeDef *reg;
    uint8_t af_tx;
    uint8_t af_rx;
};

PERIPHERAL_TYPE_EXTERN(uart);
