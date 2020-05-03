#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

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

static UART_HandleTypeDef s_uart;

void uart_init(
    resource_serial_t *instance,
    const resource_instance_decl_t *variant)
{
    __USART3_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pin = GPIO_PIN_8;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Alternate = GPIO_AF7_USART3;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

    s_uart.Instance = USART3;
    s_uart.Init = (UART_InitTypeDef) {
        .BaudRate = 115200,
        .WordLength = UART_WORDLENGTH_8B,
        .StopBits = UART_STOPBITS_1,
        .Parity = UART_PARITY_NONE,
        .Mode = USART_MODE_TX_RX,
        .HwFlowCtl = UART_HWCONTROL_NONE,
        .OverSampling = UART_OVERSAMPLING_8,
        .OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLED
    };

    if (HAL_UART_Init(&s_uart) != HAL_OK) {
        asm ("bkpt 255");
    }
}

void uart_write(
    resource_serial_t *instance,
    void *buf,
    int bytes)
{
    HAL_UART_Transmit(&s_uart, buf, bytes, HAL_MAX_DELAY);
}