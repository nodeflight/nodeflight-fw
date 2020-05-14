#include "stm32.h"
#include "hwacc/dma.h"

#include <stddef.h>

static DMA_TypeDef *dma_reg_by_id(
    uint32_t id)
{
#ifdef DMA1
    if ((id & 0xff00) == 0x0100) {
        return DMA1;
    }
#endif
#ifdef DMA2
    if ((id & 0xff00) == 0x0200) {
        return DMA2;
    }
#endif
    return NULL;
}

uint32_t dma_init_by_id(
    uint32_t id,
    LL_DMA_InitTypeDef *init_struct)
{
    DMA_TypeDef *reg = dma_reg_by_id(id);
    uint32_t stream = id & 0x000000ff;
    if (reg == NULL) {
        /* TODO: Error handling */
        return ERROR;
    }

    return LL_DMA_Init(reg, stream, init_struct);
}

void dma_set_data_length_by_id(
    uint32_t id,
    uint32_t len)
{
    DMA_TypeDef *reg = dma_reg_by_id(id);
    uint32_t stream = id & 0x000000ff;
    if (reg == NULL) {
        /* TODO: Error handling */
        return;
    }
    LL_DMA_SetDataLength(reg, stream, len);
}

void dma_disable_stream_by_id(
    uint32_t id,
    uint32_t len)
{
    DMA_TypeDef *reg = dma_reg_by_id(id);
    uint32_t stream = id & 0x000000ff;
    if (reg == NULL) {
        /* TODO: Error handling */
        return;
    }
    LL_DMA_DisableStream(reg, stream);
}

void dma_enable_stream_by_id(
    uint32_t id,
    uint32_t len)
{
    DMA_TypeDef *reg = dma_reg_by_id(id);
    uint32_t stream = id & 0x000000ff;
    if (reg == NULL) {
        /* TODO: Error handling */
        return;
    }
    LL_DMA_EnableStream(reg, stream);
    LL_DMA_EnableIT_TC(reg, stream);
}

static void dma_irq_handler(
    DMA_TypeDef *reg,
    uint32_t stream)
{
    if (LL_DMA_IsActiveFlag_TC4(reg)) {
        LL_DMA_ClearFlag_TC4(reg);
        LL_DMA_DisableStream(reg, stream);
    }
}

/* *INDENT-OFF* */
void DMA1_Stream0_IRQHandler(void) { dma_irq_handler(DMA1, LL_DMA_STREAM_0); }
void DMA1_Stream1_IRQHandler(void) { dma_irq_handler(DMA1, LL_DMA_STREAM_1); }
void DMA1_Stream2_IRQHandler(void) { dma_irq_handler(DMA1, LL_DMA_STREAM_2); }
void DMA1_Stream3_IRQHandler(void) { dma_irq_handler(DMA1, LL_DMA_STREAM_3); }
void DMA1_Stream4_IRQHandler(void) { dma_irq_handler(DMA1, LL_DMA_STREAM_4); }
void DMA1_Stream5_IRQHandler(void) { dma_irq_handler(DMA1, LL_DMA_STREAM_5); }
void DMA1_Stream6_IRQHandler(void) { dma_irq_handler(DMA1, LL_DMA_STREAM_6); }
void DMA1_Stream7_IRQHandler(void) { dma_irq_handler(DMA1, LL_DMA_STREAM_7); }
void DMA2_Stream0_IRQHandler(void) { dma_irq_handler(DMA2, LL_DMA_STREAM_0); }
void DMA2_Stream1_IRQHandler(void) { dma_irq_handler(DMA2, LL_DMA_STREAM_1); }
void DMA2_Stream2_IRQHandler(void) { dma_irq_handler(DMA2, LL_DMA_STREAM_2); }
void DMA2_Stream3_IRQHandler(void) { dma_irq_handler(DMA2, LL_DMA_STREAM_3); }
void DMA2_Stream4_IRQHandler(void) { dma_irq_handler(DMA2, LL_DMA_STREAM_4); }
void DMA2_Stream5_IRQHandler(void) { dma_irq_handler(DMA2, LL_DMA_STREAM_5); }
void DMA2_Stream6_IRQHandler(void) { dma_irq_handler(DMA2, LL_DMA_STREAM_6); }
void DMA2_Stream7_IRQHandler(void) { dma_irq_handler(DMA2, LL_DMA_STREAM_7); }
/* *INDENT-ON* */