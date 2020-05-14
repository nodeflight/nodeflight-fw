#pragma once

#include "platform/stm32/hwacc/dma.h"

#include "stm32.h"

#define DMA_ID(_DMA, _STREAM) ((_DMA) << 8 | (_STREAM))

uint32_t dma_init_by_id(
    uint32_t id,
    LL_DMA_InitTypeDef *init_struct);

void dma_set_data_length_by_id(
    uint32_t id,
    uint32_t len);

void dma_disable_stream_by_id(
    uint32_t id,
    uint32_t len);

void dma_enable_stream_by_id(
    uint32_t id,
    uint32_t len);