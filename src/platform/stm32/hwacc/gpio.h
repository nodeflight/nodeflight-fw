#pragma once

#include "core/resource.h"

#include "stm32.h"

#define GPIO_ID(_REF) ((#_REF)[0] - 'a') << 8 | (((#_REF)[1] - '0') *10 + ((#_REF)[2] - '0'))

void gpio_config_by_id(
    uint32_t id,
    LL_GPIO_InitTypeDef *init_struct);