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
#include "hwacc/dma.h"

#include <stddef.h>

typedef struct dma_stream_state_s dma_stream_state_t;

struct dma_stream_state_s {
    void *storage;
    dma_tc_callback_t tc_callback;
};

static const dma_stream_def_t dma_stream_def[16] = {
/* *INDENT-OFF* */
    [DMA_ID(1, 0)] = { .reg = DMA1, .IRQn = DMA1_Stream0_IRQn, .stream = LL_DMA_STREAM_0, .flags = &DMA1->LISR, .flag_bit =  0 },
    [DMA_ID(1, 1)] = { .reg = DMA1, .IRQn = DMA1_Stream1_IRQn, .stream = LL_DMA_STREAM_1, .flags = &DMA1->LISR, .flag_bit =  6 },
    [DMA_ID(1, 2)] = { .reg = DMA1, .IRQn = DMA1_Stream2_IRQn, .stream = LL_DMA_STREAM_2, .flags = &DMA1->LISR, .flag_bit = 16 },
    [DMA_ID(1, 3)] = { .reg = DMA1, .IRQn = DMA1_Stream3_IRQn, .stream = LL_DMA_STREAM_3, .flags = &DMA1->LISR, .flag_bit = 22 },
    [DMA_ID(1, 4)] = { .reg = DMA1, .IRQn = DMA1_Stream4_IRQn, .stream = LL_DMA_STREAM_4, .flags = &DMA1->HISR, .flag_bit =  0 },
    [DMA_ID(1, 5)] = { .reg = DMA1, .IRQn = DMA1_Stream5_IRQn, .stream = LL_DMA_STREAM_5, .flags = &DMA1->HISR, .flag_bit =  6 },
    [DMA_ID(1, 6)] = { .reg = DMA1, .IRQn = DMA1_Stream6_IRQn, .stream = LL_DMA_STREAM_6, .flags = &DMA1->HISR, .flag_bit = 16 },
    [DMA_ID(1, 7)] = { .reg = DMA1, .IRQn = DMA1_Stream7_IRQn, .stream = LL_DMA_STREAM_7, .flags = &DMA1->HISR, .flag_bit = 22 },
    [DMA_ID(2, 0)] = { .reg = DMA2, .IRQn = DMA2_Stream0_IRQn, .stream = LL_DMA_STREAM_0, .flags = &DMA2->LISR, .flag_bit =  0 },
    [DMA_ID(2, 1)] = { .reg = DMA2, .IRQn = DMA2_Stream1_IRQn, .stream = LL_DMA_STREAM_1, .flags = &DMA2->LISR, .flag_bit =  6 },
    [DMA_ID(2, 2)] = { .reg = DMA2, .IRQn = DMA2_Stream2_IRQn, .stream = LL_DMA_STREAM_2, .flags = &DMA2->LISR, .flag_bit = 16 },
    [DMA_ID(2, 3)] = { .reg = DMA2, .IRQn = DMA2_Stream3_IRQn, .stream = LL_DMA_STREAM_3, .flags = &DMA2->LISR, .flag_bit = 22 },
    [DMA_ID(2, 4)] = { .reg = DMA2, .IRQn = DMA2_Stream4_IRQn, .stream = LL_DMA_STREAM_4, .flags = &DMA2->HISR, .flag_bit =  0 },
    [DMA_ID(2, 5)] = { .reg = DMA2, .IRQn = DMA2_Stream5_IRQn, .stream = LL_DMA_STREAM_5, .flags = &DMA2->HISR, .flag_bit =  6 },
    [DMA_ID(2, 6)] = { .reg = DMA2, .IRQn = DMA2_Stream6_IRQn, .stream = LL_DMA_STREAM_6, .flags = &DMA2->HISR, .flag_bit = 16 },
    [DMA_ID(2, 7)] = { .reg = DMA2, .IRQn = DMA2_Stream7_IRQn, .stream = LL_DMA_STREAM_7, .flags = &DMA2->HISR, .flag_bit = 22 }
/* *INDENT-ON* */
};

static dma_stream_state_t dma_stream_state_def[16];

const dma_stream_def_t *dma_get(
    uint32_t id)
{
    return &dma_stream_def[id];
}

void dma_enable_irq(
    const dma_stream_def_t *def,
    uint32_t irq_priority,
    void *storage)
{
    dma_stream_state_t *state = &dma_stream_state_def[def - dma_stream_def];

    NVIC_ClearPendingIRQ(def->IRQn);
    NVIC_EnableIRQ(def->IRQn);
    NVIC_SetPriority(def->IRQn, irq_priority);

    state->storage = storage;
}

void dma_enable_tc_callback(
    const dma_stream_def_t *def,
    dma_tc_callback_t tc_callback)
{
    dma_stream_state_t *state = &dma_stream_state_def[def - dma_stream_def];
    state->tc_callback = tc_callback;
    LL_DMA_EnableIT_TC(def->reg, def->stream);
}

static void dma_irq_handler(
    const dma_stream_def_t *def,
    dma_stream_state_t *state)
{
    if (def->flags[0] & (1 << (def->flag_bit + DMA_LISR_TCIF0_Pos))) {
        def->flags[2] |= (1 << (def->flag_bit + DMA_LIFCR_CTCIF0_Pos));
        if (state->tc_callback != NULL) {
            state->tc_callback(def, state->storage);
        }
    }
}

/* *INDENT-OFF* */
void DMA1_Stream0_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 0)], &dma_stream_state_def[DMA_ID(1, 0)]); }
void DMA1_Stream1_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 1)], &dma_stream_state_def[DMA_ID(1, 1)]); }
void DMA1_Stream2_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 2)], &dma_stream_state_def[DMA_ID(1, 2)]); }
void DMA1_Stream3_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 3)], &dma_stream_state_def[DMA_ID(1, 3)]); }
void DMA1_Stream4_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 4)], &dma_stream_state_def[DMA_ID(1, 4)]); }
void DMA1_Stream5_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 5)], &dma_stream_state_def[DMA_ID(1, 5)]); }
void DMA1_Stream6_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 6)], &dma_stream_state_def[DMA_ID(1, 6)]); }
void DMA1_Stream7_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(1, 7)], &dma_stream_state_def[DMA_ID(1, 7)]); }
void DMA2_Stream0_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 0)], &dma_stream_state_def[DMA_ID(2, 0)]); }
void DMA2_Stream1_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 1)], &dma_stream_state_def[DMA_ID(2, 1)]); }
void DMA2_Stream2_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 2)], &dma_stream_state_def[DMA_ID(2, 2)]); }
void DMA2_Stream3_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 3)], &dma_stream_state_def[DMA_ID(2, 3)]); }
void DMA2_Stream4_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 4)], &dma_stream_state_def[DMA_ID(2, 4)]); }
void DMA2_Stream5_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 5)], &dma_stream_state_def[DMA_ID(2, 5)]); }
void DMA2_Stream6_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 6)], &dma_stream_state_def[DMA_ID(2, 6)]); }
void DMA2_Stream7_IRQHandler(void) { dma_irq_handler(&dma_stream_def[DMA_ID(2, 7)], &dma_stream_state_def[DMA_ID(2, 7)]); }
/* *INDENT-ON* */
