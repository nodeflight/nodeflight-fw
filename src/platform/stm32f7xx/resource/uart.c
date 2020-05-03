#include "stm32f7xx.h"

#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_usart.h"

#include "resource/resource.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct uart_storage_s {
    resource_serial_t header;
} uart_storage_t;

static void uart_init(
    resource_serial_t *instance,
    const resource_instance_decl_t *variant);

static void uart_write(
    resource_serial_t *instance,
    void *buf,
    int bytes);

static const resource_serial_t uart_callbacks = {
    .init = uart_init,
    .write = uart_write
};

RESOURCE_TYPE_DECL(uart, RESOURCE_SERIAL, uart_storage_t, uart_callbacks);

RESOURCE_INSTANCE_DECL(uart, uart1_a09_a10, "UART1 @ A09 A10", NULL);
RESOURCE_INSTANCE_DECL(uart, uart1_b06_b07, "UART1 @ B06 B07", NULL);
RESOURCE_INSTANCE_DECL(uart, uart2_a02_a03, "UART2 @ A02 A03", NULL);
RESOURCE_INSTANCE_DECL(uart, uart2_d05_d06, "UART2 @ D05 D06", NULL);
RESOURCE_INSTANCE_DECL(uart, uart3_b10_b11, "UART3 @ B10 B11", NULL);
RESOURCE_INSTANCE_DECL(uart, uart3_c10_c11, "UART3 @ C10 C11", NULL);
RESOURCE_INSTANCE_DECL(uart, uart3_d08_d09, "UART3 @ D08 D09", NULL);
RESOURCE_INSTANCE_DECL(uart, uart4_c10_c11, "UART4 @ C10 C11", NULL);
RESOURCE_INSTANCE_DECL(uart, uart6_c06_c07, "UART6 @ C06 C07", NULL);
RESOURCE_INSTANCE_DECL(uart, uart6_g14_g09, "UART6 @ G14 G09", NULL);

void uart_init(
    resource_serial_t *instance,
    const resource_instance_decl_t *variant)
{
    LL_GPIO_Init(GPIOD, &(LL_GPIO_InitTypeDef) {
        .Pin = LL_GPIO_PIN_8,
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = LL_GPIO_AF_7
    });

    LL_GPIO_Init(GPIOD, &(LL_GPIO_InitTypeDef) {
        .Pin = LL_GPIO_PIN_9,
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_OPENDRAIN,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = LL_GPIO_AF_7
    });

    LL_GPIO_Init(GPIOD, &(LL_GPIO_InitTypeDef) {
        .Pin = LL_GPIO_PIN_9,
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_OPENDRAIN,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = LL_GPIO_AF_7
    });

    LL_USART_Init(USART3, &(LL_USART_InitTypeDef) {
        .BaudRate = 115200,
        .DataWidth = LL_USART_DATAWIDTH_8B,
        .StopBits = LL_USART_STOPBITS_1,
        .Parity = LL_USART_PARITY_NONE,
        .TransferDirection = LL_USART_DIRECTION_TX_RX,
        .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
        .OverSampling = LL_USART_OVERSAMPLING_16
    });
    LL_USART_Enable(USART3);
}

void uart_write(
    resource_serial_t *instance,
    void *buf,
    int bytes)
{
    uint8_t *cur = buf;
    while (bytes--) {
        while (!LL_USART_IsActiveFlag_TXE(USART3)) {
        }
        LL_USART_TransmitData8(USART3, *(cur++));
    }
    while (!LL_USART_IsActiveFlag_TC(USART3)) {
    }
}