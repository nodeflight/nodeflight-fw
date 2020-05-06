#include "stm32.h"
#include "core/strops.h"
#include "hwacc/peripheral.h"
#include "hwacc/resource.h"
#include "hwacc/interface.h"
#include "platform/stm32/hwacc/uart.h"
#include "platform/stm32/hwacc/gpio.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct uart_storage_s {
    peripheral_serial_t header;

    USART_TypeDef *reg;

    interface_resource_t *rscs;
} uart_storage_t;

static void uart_init(
    peripheral_serial_t *instance,
    const peripheral_instance_decl_t *variant,
    const char *tag);

static void uart_write(
    peripheral_serial_t *instance,
    void *buf,
    int bytes);

static const peripheral_serial_t uart_callbacks = {
    .init = uart_init,
    .write = uart_write
};

PERIPHERAL_TYPE_DECL(uart, PERIPHERAL_SERIAL, 2, uart_storage_t, uart_callbacks);

void uart_init(
    peripheral_serial_t *instance,
    const peripheral_instance_decl_t *variant,
    const char *tag)
{
    uart_storage_t *storage = (uart_storage_t *) instance;

    const char *cur_tag = tag;

    /* Skip peripheral name */
    strops_next_word(&cur_tag);

    storage->rscs = interface_resource_allocate(variant, &cur_tag);

    if (storage->rscs == NULL) {
        return;
    }

    storage->reg = variant->storage;

    gpio_config_by_id(storage->rscs[0].decl->ref, &(LL_GPIO_InitTypeDef) {
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = storage->rscs[0].inst->attr
    });

    gpio_config_by_id(storage->rscs[1].decl->ref, &(LL_GPIO_InitTypeDef) {
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_OPENDRAIN,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = storage->rscs[1].inst->attr
    });

    LL_USART_Init(storage->reg, &(LL_USART_InitTypeDef) {
        .BaudRate = 115200,
        .DataWidth = LL_USART_DATAWIDTH_8B,
        .StopBits = LL_USART_STOPBITS_1,
        .Parity = LL_USART_PARITY_NONE,
        .TransferDirection = LL_USART_DIRECTION_TX_RX,
        .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
        .OverSampling = LL_USART_OVERSAMPLING_16
    });
    LL_USART_Enable(storage->reg);
}

void uart_write(
    peripheral_serial_t *instance,
    void *buf,
    int bytes)
{
    uart_storage_t *storage = (uart_storage_t *) instance;
    uint8_t *cur = buf;
    while (bytes--) {
        while (!LL_USART_IsActiveFlag_TXE(storage->reg)) {
        }
        LL_USART_TransmitData8(storage->reg, *(cur++));
    }
    while (!LL_USART_IsActiveFlag_TC(storage->reg)) {
    }
}