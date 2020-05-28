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

#pragma once

#include "platform/stm32/resource/dma.h"

#include "stm32.h"

#define DMA_ID(_DMA, _STREAM) (((_DMA) -1) << 3 | (_STREAM))
#define DMA_ID_NONE 0xffffffff

typedef struct dma_stream_def_s dma_stream_def_t;

typedef void (*dma_callback_t)(
    const dma_stream_def_t *def,
    void *storage);

struct dma_stream_def_s {
    DMA_TypeDef *reg;
    uint32_t IRQn;
    uint32_t stream;

    /* For use by dma interrupt handlers */
    volatile uint32_t *flags;
    uint8_t flag_bit;
};

const dma_stream_def_t *dma_get(
    uint32_t id);

void dma_enable_irq(
    const dma_stream_def_t *def,
    uint32_t irq_priority,
    void *storage);

void dma_set_fifo_error_cb(
    const dma_stream_def_t *def,
    dma_callback_t callback);

void dma_set_direct_mode_error_cb(
    const dma_stream_def_t *def,
    dma_callback_t callback);

void dma_set_transfer_error_cb(
    const dma_stream_def_t *def,
    dma_callback_t callback);

void dma_set_half_transfer_cb(
    const dma_stream_def_t *def,
    dma_callback_t callback);

void dma_set_transfer_complete_cb(
    const dma_stream_def_t *def,
    dma_callback_t callback);
