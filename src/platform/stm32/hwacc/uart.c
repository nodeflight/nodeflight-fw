#include "stm32.h"
#include "core/strops.h"
#include "hwacc/peripheral.h"
#include "hwacc/resource.h"
#include "hwacc/interface.h"
#include "hwacc/interface_types.h"
#include "platform/stm32/hwacc/uart.h"
#include "platform/stm32/hwacc/gpio.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct uart_interface_s {
    interface_serial_t header;
    USART_TypeDef *reg;
} uart_interface_t;

int uart_init(
    interface_header_t *iface,
    const char *config);

static int uart_write(
    interface_serial_t *iface,
    void *buf,
    int bytes);

PERIPHERAL_TYPE_DECL(uart, PERIPHERAL_SERIAL, 4, uart_init, sizeof(uart_interface_t));

int uart_init(
    interface_header_t *iface,
    const char *config)
{
    uart_interface_t *if_uart = (uart_interface_t *) iface;
    interface_resource_t *rscs = if_uart->header.header.rscs;

    if_uart->header.write = uart_write;
    if_uart->reg = if_uart->header.header.peripheral->storage;

    gpio_config_by_id(rscs[0].decl->ref, &(LL_GPIO_InitTypeDef) {
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = rscs[0].inst->attr
    });

    gpio_config_by_id(rscs[1].decl->ref, &(LL_GPIO_InitTypeDef) {
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_OPENDRAIN,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = rscs[1].inst->attr
    });

    LL_USART_Init(if_uart->reg, &(LL_USART_InitTypeDef) {
        .BaudRate = 115200,
        .DataWidth = LL_USART_DATAWIDTH_8B,
        .StopBits = LL_USART_STOPBITS_1,
        .Parity = LL_USART_PARITY_NONE,
        .TransferDirection = LL_USART_DIRECTION_TX_RX,
        .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
        .OverSampling = LL_USART_OVERSAMPLING_16
    });
    LL_USART_Enable(if_uart->reg);

    return 0;
}

static int uart_write(
    interface_serial_t *iface,
    void *buf,
    int bytes)
{
    uart_interface_t *if_uart = (uart_interface_t *) iface;
    uint8_t *cur = buf;
    while (bytes--) {
        while (!LL_USART_IsActiveFlag_TXE(if_uart->reg)) {
        }
        LL_USART_TransmitData8(if_uart->reg, *(cur++));
    }
    while (!LL_USART_IsActiveFlag_TC(if_uart->reg)) {
    }
    return bytes;
}