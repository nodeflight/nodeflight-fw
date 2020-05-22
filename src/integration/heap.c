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

/*
 * Integration for freertos heap.
 *
 * Quite similar to heap_1, but uses symbols from linker to allocate memory for the buffer
 */

#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE
#include "FreeRTOS.h"
#include "task.h"
#undef  MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "integration/heap.h"

#define ALIGN_BYTES 4

#if ALIGN_BYTES & (ALIGN_BYTES - 1)
#error ALIGN_BYTES must be a power of 2
#endif

#if configUSE_MALLOC_FAILED_HOOK
extern void vApplicationMallocFailedHook(
    void);
#endif

/* Heap defined by linker script */
extern uint8_t __heap_start[];
extern uint8_t __heap_end[];

static uint8_t *cur_alloc;

void *pvPortMalloc(
    size_t xWantedSize)
{
    uint8_t *block = NULL;
    size_t req_size;

    /* Always allocate aligned */
    req_size = (xWantedSize + portBYTE_ALIGNMENT_MASK) & ~portBYTE_ALIGNMENT_MASK;

    vTaskSuspendAll();

    uint8_t *next_block = cur_alloc + req_size;

    if (next_block <= __heap_end) {
        /* Successful allocation */
        block = cur_alloc;
        cur_alloc = next_block;

        traceMALLOC(pvReturn, xWantedSize);
    }

    xTaskResumeAll();

#if configUSE_MALLOC_FAILED_HOOK
    if (block == NULL) {
        vApplicationMallocFailedHook();
    }
#endif

    return (void *) block;
}

void vPortFree(
    void *pv)
{
    (void) pv;
    configASSERT(pv == NULL);
}

void vPortInitialiseBlocks(
    void)
{
    cur_alloc = __heap_start;
}

size_t xPortGetFreeHeapSize(
    void)
{
    return __heap_end - cur_alloc;
}

uint32_t heap_get_size(
    void)
{
    return __heap_end - __heap_start;
}

uint32_t heap_get_usage(
    void)
{
    return cur_alloc - __heap_start;
}
