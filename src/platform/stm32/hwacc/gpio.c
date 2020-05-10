#include "platform/stm32/hwacc/gpio.h"
#include "hwacc/resource.h"

#include "stm32.h"

#include <stddef.h>

static GPIO_TypeDef *const gpio_ports[] = {
#ifdef GPIOA
    [0] = GPIOA,
#endif
#ifdef GPIOB
    [1] = GPIOB,
#endif
#ifdef GPIOC
    [2] = GPIOC,
#endif
#ifdef GPIOD
    [3] = GPIOD,
#endif
#ifdef GPIOE
    [4] = GPIOE,
#endif
#ifdef GPIOF
    [5] = GPIOF,
#endif
#ifdef GPIOG
    [6] = GPIOG,
#endif
#ifdef GPIOH
    [7] = GPIOH,
#endif
#ifdef GPIOI
    [8] = GPIOI,
#endif
#ifdef GPIOJ
    [9] = GPIOJ,
#endif
#ifdef GPIOK
    [10] = GPIOK,
#endif
};

void gpio_config_by_id(
    uint32_t id,
    LL_GPIO_InitTypeDef *init_struct)
{
    init_struct->Pin = 1 << (id & 0x000000ff);
    LL_GPIO_Init(gpio_ports[(id & 0x0000ff00) >> 8], init_struct);
}