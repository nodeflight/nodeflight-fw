#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

#include "platform/platform.h"

void SysTick_Handler(
    void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void platform_init(
    void)
{

    HAL_Init();

}