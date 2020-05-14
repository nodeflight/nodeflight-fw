#pragma once

#include "platform/stm32/hwacc/dma.h"

#include "stm32.h"

#define DMA_ID(_DMA, _STREAM) (((_DMA) -1) << 3 | (_STREAM))

typedef struct dma_stream_def_s dma_stream_def_t;

typedef void (*dma_tc_callback_t)(
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

void dma_enable_tc_callback(
    const dma_stream_def_t *def,
    dma_tc_callback_t tc_callback);