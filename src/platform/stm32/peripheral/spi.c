/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2023  Max Sikström
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
#include "core/interface.h"
#include "core/interface_types.h"
#include "platform/stm32/peripheral/spi.h"
#include "platform/stm32/resource/gpio.h"
#include "platform/stm32/resource/dma.h"

#include "stm32.h"

#include "core/log.h"

#include "FreeRTOS.h"
#include "task.h"

#define SPI_IRQ_PRIORITY 5

typedef struct spi_if_s spi_if_t;

struct spi_if_s {
    if_spi_t header;
    spi_def_t def;

    uint8_t *tx_ptr;
    int tx_left;

    uint8_t *rx_ptr;
    int rx_left;

    TaskHandle_t current_task;
};

static int spi_init(
    if_header_t *iface);

static int spi_configure(
    if_spi_t *iface,
    const if_spi_cf_t *config);

static void spi_transfer(
    if_spi_t *iface,
    uint8_t *tx_buf,
    uint8_t *rx_buf,
    int length);

static spi_if_t *spi_interfaces[SPI_MAX_COUNT] = {
    0
};

PP_TYPE_DECL(
    spi,
    PP_SPI,
    SPI_NUM_ARGS,
    spi_init,
    sizeof(spi_if_t));

int spi_init(
    if_header_t *iface)
{
    spi_if_t *spi = (spi_if_t *) iface;
    spi->def = *((const spi_def_t *) spi->header.header.peripheral->storage);
    spi->header.configure = spi_configure;
    spi->header.transfer = spi_transfer;

    return 0;
}

static uint32_t spi_calc_baud_rate(
    uint32_t baudrate_hz)
{
    uint32_t baudrate_bits[8] = {
        LL_SPI_BAUDRATEPRESCALER_DIV2,
        LL_SPI_BAUDRATEPRESCALER_DIV4,
        LL_SPI_BAUDRATEPRESCALER_DIV8,
        LL_SPI_BAUDRATEPRESCALER_DIV16,
        LL_SPI_BAUDRATEPRESCALER_DIV32,
        LL_SPI_BAUDRATEPRESCALER_DIV64,
        LL_SPI_BAUDRATEPRESCALER_DIV128,
        LL_SPI_BAUDRATEPRESCALER_DIV256
    };
    /* TODO: use correct clock source */
    uint32_t current_baudrate = SystemCoreClock;
    int idx = 0;
    current_baudrate >>= 1;
    while (current_baudrate > baudrate_hz && idx < 7) {
        idx++;
        current_baudrate >>= 1;
    }
    return baudrate_bits[idx];
}

int spi_configure(
    if_spi_t *iface,
    const if_spi_cf_t *config)
{
    spi_if_t *spi = (spi_if_t *) iface;
    if_rs_t *rscs = spi->header.header.rscs;

    LL_SPI_Disable(spi->def.reg);

    /* TODO: Enable transfer via DMA */

    LL_SPI_InitTypeDef spi_conf = {
        .TransferDirection = LL_SPI_FULL_DUPLEX,
        .Mode = LL_SPI_MODE_MASTER,
        .DataWidth = LL_SPI_DATAWIDTH_8BIT,
        .NSS = LL_SPI_NSS_SOFT,
        .BitOrder = LL_SPI_MSB_FIRST,
        .CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE,
        .CRCPoly = 0x0000
    };

    spi_conf.BaudRate = spi_calc_baud_rate(config->max_baud_rate);

    switch (config->mode) {
    case SPI_MODE_LEADING_HIGH:
        spi_conf.ClockPolarity = LL_SPI_POLARITY_HIGH;
        spi_conf.ClockPhase = LL_SPI_PHASE_1EDGE;
        break;

    case SPI_MODE_LEADING_LOW:
        spi_conf.ClockPolarity = LL_SPI_POLARITY_LOW;
        spi_conf.ClockPhase = LL_SPI_PHASE_1EDGE;
        break;

    case SPI_MODE_TRAILING_HIGH:
        spi_conf.ClockPolarity = LL_SPI_POLARITY_HIGH;
        spi_conf.ClockPhase = LL_SPI_PHASE_2EDGE;
        break;

    case SPI_MODE_TRAILING_LOW:
        spi_conf.ClockPolarity = LL_SPI_POLARITY_LOW;
        spi_conf.ClockPhase = LL_SPI_PHASE_2EDGE;
        break;
    }

    spi_interfaces[spi->def.id] = spi;
    LL_SPI_Init(spi->def.reg, &spi_conf);
    /* Receive in 8 bit blocks */
    LL_SPI_SetRxFIFOThreshold(spi->def.reg, LL_SPI_RX_FIFO_TH_QUARTER);

    gpio_configure_alternative(&rscs[SPI_ARG_PIN_SCK]);
    gpio_configure_alternative(&rscs[SPI_ARG_PIN_MOSI]);
    gpio_configure_alternative(&rscs[SPI_ARG_PIN_MISO]);

    LL_SPI_Enable(spi->def.reg);

    NVIC_ClearPendingIRQ(spi->def.IRQn);
    NVIC_SetPriority(spi->def.IRQn, SPI_IRQ_PRIORITY);
    NVIC_EnableIRQ(spi->def.IRQn);

    return 0;
}

void spi_transfer(
    if_spi_t *iface,
    uint8_t *tx_buf,
    uint8_t *rx_buf,
    int length)
{
    spi_if_t *spi = (spi_if_t *) iface;
    uint32_t notify_value = 0;

    spi->current_task = xTaskGetCurrentTaskHandle();

    /* Get to a defined state, flush input buffer */
    while (LL_SPI_IsActiveFlag_RXNE(spi->def.reg)) {
        LL_SPI_ReceiveData8(spi->def.reg);
    }

    spi->tx_ptr = tx_buf;
    spi->tx_left = length;
    spi->rx_ptr = rx_buf;
    spi->rx_left = length;

    LL_SPI_EnableIT_RXNE(spi->def.reg);
    LL_SPI_EnableIT_TXE(spi->def.reg);

    do{
        xTaskNotifyWait(0, 0xff000000, &notify_value, (1000 * portTICK_PERIOD_MS));
    } while(!(notify_value & 0x01000000));

    spi->current_task = NULL;
}

volatile int spi1_tx_count, spi1_rx_count;
uint8_t *spi1_tx_ptr;
volatile uint8_t *spi1_rx_ptr;
/* set these global variables before enabling interrupts */

static void spi_notify(
    spi_if_t *spi)
{
    if (spi->current_task != NULL) {
        BaseType_t should_switch = pdFALSE;
        xTaskNotifyFromISR(spi->current_task, 0x01000000, eSetBits, &should_switch);
        portYIELD_FROM_ISR(should_switch);
    }
}

static void spi_irqhandler(
    SPI_TypeDef *reg,
    spi_if_t *spi)
{
    while (LL_SPI_IsActiveFlag_RXNE(reg)) {
        if (spi->rx_left == 0) {
            LL_SPI_DisableIT_RXNE(reg);
        } else {
            uint8_t rx_data = LL_SPI_ReceiveData8(reg);
            if (spi->rx_ptr != NULL) {
                *spi->rx_ptr = rx_data;
                spi->rx_ptr++;
            }
            spi->rx_left--;
        }
    }
    if (LL_SPI_IsActiveFlag_TXE(reg)) {
        if (spi->tx_left == 0) {
            LL_SPI_DisableIT_TXE(reg);
        } else {
            if (spi->tx_ptr != NULL) {
                LL_SPI_TransmitData8(reg, *spi->tx_ptr);
                spi->tx_ptr++;
            } else {
                LL_SPI_TransmitData8(reg, 0xff);
            }
            spi->tx_left--;
        }
    }
    if (spi->tx_left == 0 && spi->rx_left == 0) {
        LL_SPI_DisableIT_TXE(reg);
        LL_SPI_DisableIT_RXNE(reg);
        spi_notify(spi);
    }
}

/* *INDENT-OFF* */
#ifdef SPI1
void SPI1_IRQHandler(void) { spi_irqhandler(SPI1, spi_interfaces[0]); }
#endif
#ifdef SPI2
void SPI2_IRQHandler(void) { spi_irqhandler(SPI2, spi_interfaces[1]); }
#endif
#ifdef SPI3
void SPI3_IRQHandler(void) { spi_irqhandler(SPI3, spi_interfaces[2]); }
#endif
#ifdef SPI4
void SPI4_IRQHandler(void) { spi_irqhandler(SPI4, spi_interfaces[3]); }
#endif
#ifdef SPI5
void SPI5_IRQHandler(void) { spi_irqhandler(SPI5, spi_interfaces[4]); }
#endif
/* *INDENT-ON* */
