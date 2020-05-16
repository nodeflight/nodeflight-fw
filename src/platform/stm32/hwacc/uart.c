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

#include "stm32.h"
#include "lib/strops.h"
#include "core/peripheral.h"
#include "core/resource.h"
#include "core/interface.h"
#include "core/interface_types.h"
#include "platform/stm32/hwacc/uart.h"
#include "platform/stm32/hwacc/gpio.h"
#include "platform/stm32/hwacc/dma.h"

#include "FreeRTOS.h"

#include <stddef.h>
#include <string.h>

typedef struct uart_interface_s {
    interface_serial_t header;
    USART_TypeDef *reg;

    const dma_stream_def_t *tx_dma;
    uint8_t *tx_buf;
    uint16_t tx_buf_size;
} uart_interface_t;

static int uart_init(
    interface_header_t *iface,
    const char *config);

static int uart_configure(
    interface_serial_t *iface,
    const interface_serial_config_t *config);

static int uart_tx_write(
    interface_serial_t *iface,
    const void *buf,
    int bytes);

static void uart_tx_wait_done(
    interface_serial_t *iface);

PERIPHERAL_TYPE_DECL(uart, PERIPHERAL_SERIAL, UART_NUM_ARGS, uart_init, sizeof(uart_interface_t));

static void uart_tx_tc_callback(
    const dma_stream_def_t *def,
    void *storage)
{
    LL_DMA_DisableStream(def->reg, def->stream);
}

int uart_init(
    interface_header_t *iface,
    const char *config)
{
    uart_interface_t *if_uart = (uart_interface_t *) iface;

    if_uart->header.configure = uart_configure;
    if_uart->header.tx_write = uart_tx_write;
    if_uart->header.tx_wait_done = uart_tx_wait_done;

    if_uart->reg = if_uart->header.header.peripheral->storage;

    if_uart->tx_dma = NULL;
    if_uart->tx_buf = NULL;
    if_uart->tx_buf_size = 0;

    return 0;
}

int uart_configure(
    interface_serial_t *iface,
    const interface_serial_config_t *config)
{
    uart_interface_t *if_uart = (uart_interface_t *) iface;
    interface_resource_t *rscs = if_uart->header.header.rscs;

    bool tx_en = rscs[UART_ARG_PIN_TX].decl->ref != GPIO_ID_NONE && rscs[UART_ARG_DMA_TX].decl->ref != DMA_ID_NONE;
    bool rx_en = rscs[UART_ARG_PIN_RX].decl->ref != GPIO_ID_NONE && rscs[UART_ARG_DMA_RX].decl->ref != DMA_ID_NONE;

    LL_USART_Init(if_uart->reg, &(LL_USART_InitTypeDef) {
        .BaudRate = config->baudrate,
        .DataWidth = LL_USART_DATAWIDTH_8B,
        .StopBits = LL_USART_STOPBITS_1,
        .Parity = LL_USART_PARITY_NONE,
        .TransferDirection = (
            (tx_en && rx_en) ? LL_USART_DIRECTION_TX_RX
            : (tx_en && !rx_en) ? LL_USART_DIRECTION_TX
            : (!tx_en && rx_en) ? LL_USART_DIRECTION_RX
            : LL_USART_DIRECTION_NONE
            ),
        .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
        .OverSampling = LL_USART_OVERSAMPLING_16
    });

    /* TX */
    if (tx_en) {
        if_uart->tx_dma = dma_get(rscs[UART_ARG_DMA_TX].decl->ref);
        if_uart->tx_buf = pvPortMalloc(config->tx_buf_size);
        if_uart->tx_buf_size = config->tx_buf_size;

        gpio_config_by_id(rscs[UART_ARG_PIN_TX].decl->ref, &(LL_GPIO_InitTypeDef) {
            .Mode = LL_GPIO_MODE_ALTERNATE,
            .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
            .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
            .Pull = LL_GPIO_PULL_NO,
            .Alternate = rscs[UART_ARG_PIN_TX].inst->attr
        });
        LL_USART_SetTXPinLevel(if_uart->reg,
            (config->flags & INTERFACE_SERIAL_INVERTED_TX)
            ? LL_USART_TXPIN_LEVEL_INVERTED
            : LL_USART_TXPIN_LEVEL_STANDARD
        );

        LL_DMA_Init(if_uart->tx_dma->reg, if_uart->tx_dma->stream, &(LL_DMA_InitTypeDef) {
            .PeriphOrM2MSrcAddress = LL_USART_DMA_GetRegAddr(if_uart->reg, LL_USART_DMA_REG_DATA_TRANSMIT),
            .MemoryOrM2MDstAddress = (uint32_t) if_uart->tx_buf,
            .Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH,
            .Mode = LL_DMA_MODE_NORMAL, // LL_DMA_MODE_CIRCULAR,
            .PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT,
            .MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT,
            .PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE,
            .MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE,
            .NbData = 0,
            .Channel = rscs[UART_ARG_DMA_TX].inst->attr << DMA_SxCR_CHSEL_Pos,
            .Priority = LL_DMA_PRIORITY_MEDIUM,
            .FIFOMode = LL_DMA_FIFOMODE_DISABLE,
            .FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_2,
            .MemBurst = LL_DMA_MBURST_SINGLE,
            .PeriphBurst = LL_DMA_PBURST_SINGLE
        });

        dma_enable_irq(if_uart->tx_dma, 5, if_uart);
        dma_set_transfer_complete_cb(if_uart->tx_dma, uart_tx_tc_callback);
        LL_USART_EnableDMAReq_TX(if_uart->reg);
    }

    /* RX */
    if (rx_en) {
        gpio_config_by_id(rscs[UART_ARG_PIN_RX].decl->ref, &(LL_GPIO_InitTypeDef) {
            .Mode = LL_GPIO_MODE_ALTERNATE,
            .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
            .OutputType = LL_GPIO_OUTPUT_OPENDRAIN,
            .Pull = LL_GPIO_PULL_NO,
            .Alternate = rscs[UART_ARG_PIN_RX].inst->attr
        });

        LL_USART_SetRXPinLevel(if_uart->reg,
            (config->flags & INTERFACE_SERIAL_INVERTED_RX)
            ? LL_USART_RXPIN_LEVEL_INVERTED
            : LL_USART_RXPIN_LEVEL_STANDARD
        );

    }

    LL_USART_Enable(if_uart->reg);

    return 0;
}

int uart_tx_write(
    interface_serial_t *iface,
    const void *buf,
    int bytes)
{
    int i;
    uart_interface_t *if_uart = (uart_interface_t *) iface;

    if (if_uart->tx_buf == NULL) {
        return -1;
    }

    for (i = 0; i < bytes; i += if_uart->tx_buf_size) {
        int cur_bytes = bytes - i;
        if (cur_bytes > if_uart->tx_buf_size) {
            cur_bytes = if_uart->tx_buf_size;
        }
        /* Block if already transmitting */
        while (LL_DMA_IsEnabledStream(if_uart->tx_dma->reg, if_uart->tx_dma->stream)) {
        }
        memcpy(if_uart->tx_buf, ((const uint8_t *) buf) + i, cur_bytes);
        LL_DMA_SetDataLength(if_uart->tx_dma->reg, if_uart->tx_dma->stream, cur_bytes);
        LL_DMA_EnableStream(if_uart->tx_dma->reg, if_uart->tx_dma->stream);
        LL_USART_ClearFlag_TC(if_uart->reg);
    }

    return bytes;
}

void uart_tx_wait_done(
    interface_serial_t *iface)
{
    uart_interface_t *if_uart = (uart_interface_t *) iface;
    while (!LL_USART_IsActiveFlag_TC(if_uart->reg)) {
    }
}
