#include "stm32.h"

#include "hwacc/peripheral.h"
#include "hwacc/resource.h"
#include "platform/stm32/hwacc/uart.h"
#include "platform/stm32/hwacc/gpio.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct uart_storage_s {
    peripheral_serial_t header;

    const uart_hwconf_t *hwconf;
    const resource_decl_t *res_tx;
    const resource_decl_t *res_rx;
} uart_storage_t;

static void uart_init(
    peripheral_serial_t *instance,
    const peripheral_instance_decl_t *variant);

static void uart_write(
    peripheral_serial_t *instance,
    void *buf,
    int bytes);

static const peripheral_serial_t uart_callbacks = {
    .init = uart_init,
    .write = uart_write
};

PERIPHERAL_TYPE_DECL(uart, PERIPHERAL_SERIAL, uart_storage_t, uart_callbacks);

void uart_init(
    peripheral_serial_t *instance,
    const peripheral_instance_decl_t *variant)
{
    uart_storage_t *storage = (uart_storage_t *)instance;
    storage->hwconf = (const uart_hwconf_t *) variant->storage;

    const char *cur_res = variant->resources;
    storage->res_tx = resource_get_by_name_r(&cur_res);
    storage->res_rx = resource_get_by_name_r(&cur_res);

    if (storage->res_tx == NULL || storage->res_rx == NULL) {
        return;
    }

    gpio_config_by_id(storage->res_tx->ref, &(LL_GPIO_InitTypeDef) {
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = storage->hwconf->af_tx
    });

    gpio_config_by_id(storage->res_rx->ref, &(LL_GPIO_InitTypeDef) {
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_OPENDRAIN,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = storage->hwconf->af_rx
    });

    LL_USART_Init(storage->hwconf->reg, &(LL_USART_InitTypeDef) {
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
    peripheral_serial_t *instance,
    void *buf,
    int bytes)
{
    uart_storage_t *storage = (uart_storage_t *)instance;
    uint8_t *cur = buf;
    while (bytes--) {
        while (!LL_USART_IsActiveFlag_TXE(storage->hwconf->reg)) {
        }
        LL_USART_TransmitData8(storage->hwconf->reg, *(cur++));
    }
    while (!LL_USART_IsActiveFlag_TC(storage->hwconf->reg)) {
    }
}