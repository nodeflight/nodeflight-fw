#include "stm32.h"
#include "hwacc/dma.h"

#include <stddef.h>

static const dma_stream_def_t dma_stream_def[16] = {
    [DMA_ID(1, 0)] = { .reg = DMA1, .IRQn = DMA1_Stream0_IRQn, .stream = LL_DMA_STREAM_0 },
    [DMA_ID(1, 1)] = { .reg = DMA1, .IRQn = DMA1_Stream1_IRQn, .stream = LL_DMA_STREAM_1 },
    [DMA_ID(1, 2)] = { .reg = DMA1, .IRQn = DMA1_Stream2_IRQn, .stream = LL_DMA_STREAM_2 },
    [DMA_ID(1, 3)] = { .reg = DMA1, .IRQn = DMA1_Stream3_IRQn, .stream = LL_DMA_STREAM_3 },
    [DMA_ID(1, 4)] = { .reg = DMA1, .IRQn = DMA1_Stream4_IRQn, .stream = LL_DMA_STREAM_4 },
    [DMA_ID(1, 5)] = { .reg = DMA1, .IRQn = DMA1_Stream5_IRQn, .stream = LL_DMA_STREAM_5 },
    [DMA_ID(1, 6)] = { .reg = DMA1, .IRQn = DMA1_Stream6_IRQn, .stream = LL_DMA_STREAM_6 },
    [DMA_ID(1, 7)] = { .reg = DMA1, .IRQn = DMA1_Stream7_IRQn, .stream = LL_DMA_STREAM_7 },
    [DMA_ID(2, 0)] = { .reg = DMA2, .IRQn = DMA2_Stream0_IRQn, .stream = LL_DMA_STREAM_0 },
    [DMA_ID(2, 1)] = { .reg = DMA2, .IRQn = DMA2_Stream1_IRQn, .stream = LL_DMA_STREAM_1 },
    [DMA_ID(2, 2)] = { .reg = DMA2, .IRQn = DMA2_Stream2_IRQn, .stream = LL_DMA_STREAM_2 },
    [DMA_ID(2, 3)] = { .reg = DMA2, .IRQn = DMA2_Stream3_IRQn, .stream = LL_DMA_STREAM_3 },
    [DMA_ID(2, 4)] = { .reg = DMA2, .IRQn = DMA2_Stream4_IRQn, .stream = LL_DMA_STREAM_4 },
    [DMA_ID(2, 5)] = { .reg = DMA2, .IRQn = DMA2_Stream5_IRQn, .stream = LL_DMA_STREAM_5 },
    [DMA_ID(2, 6)] = { .reg = DMA2, .IRQn = DMA2_Stream6_IRQn, .stream = LL_DMA_STREAM_6 },
    [DMA_ID(2, 7)] = { .reg = DMA2, .IRQn = DMA2_Stream7_IRQn, .stream = LL_DMA_STREAM_7 }
};

const dma_stream_def_t *dma_get(
    uint32_t id)
{
    return &dma_stream_def[id];
}

void dma_enable_irq(
    const dma_stream_def_t *def,
    uint32_t irq_priority)
{
    NVIC_ClearPendingIRQ(def->IRQn);
    NVIC_EnableIRQ(def->IRQn);
    NVIC_SetPriority(def->IRQn, irq_priority);
    LL_DMA_EnableIT_TC(def->reg, def->stream);
}

static void dma_irq_handler(
    const dma_stream_def_t *def)
{
    if (LL_DMA_IsActiveFlag_TC4(def->reg)) {
        LL_DMA_ClearFlag_TC4(def->reg);
        LL_DMA_DisableStream(def->reg, def->stream);
    }
}

/* *INDENT-OFF* */
void DMA1_Stream0_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 0)]); }
void DMA1_Stream1_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 1)]); }
void DMA1_Stream2_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 2)]); }
void DMA1_Stream3_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 3)]); }
void DMA1_Stream4_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 4)]); }
void DMA1_Stream5_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 5)]); }
void DMA1_Stream6_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 6)]); }
void DMA1_Stream7_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 7)]); }
void DMA2_Stream0_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 0)]); }
void DMA2_Stream1_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 1)]); }
void DMA2_Stream2_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 2)]); }
void DMA2_Stream3_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 3)]); }
void DMA2_Stream4_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 4)]); }
void DMA2_Stream5_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 5)]); }
void DMA2_Stream6_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 6)]); }
void DMA2_Stream7_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 7)]); }
/* *INDENT-ON* */