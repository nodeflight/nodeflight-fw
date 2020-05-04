#include "stm32.h"

#include "platform/platform.h"

void platform_init(
    void)
{
    /* Don't use priority grouping... */
    NVIC_SetPriorityGrouping(0);

    /* Enable all peripheral clocks. Current consumption can be optimized later */
    RCC->AHB1ENR = 0xffffffff;
    RCC->AHB2ENR = 0xffffffff;
    RCC->AHB3ENR = 0xffffffff;
    RCC->APB1ENR = 0xffffffff;
    RCC->APB2ENR = 0xffffffff;
    __DSB();
}