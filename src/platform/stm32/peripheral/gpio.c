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

#include "core/peripheral.h"
#include "core/interface.h"
#include "core/interface_types.h"
#include "platform/stm32/peripheral/gpio.h"
#include "platform/stm32/resource/gpio.h"

#include "stm32.h"

#define EXTI_IRQ_PRIORITY 7

typedef struct gpio_if_s gpio_if_t;
typedef struct gpio_edge_state_s gpio_edge_state_t;

struct gpio_if_s {
    if_gpio_t header;
    uint32_t pin_id;
};

struct gpio_edge_state_s {
    void (*edge_callback)(
        void *storage);
    void *edge_storage;
};

static int gpio_init(
    if_header_t *iface);

static int gpio_if_configure(
    if_gpio_t *iface,
    const if_gpio_cf_t *config);

static void gpio_if_set_value(
    if_gpio_t *iface,
    bool value);

static bool gpio_if_get_value(
    if_gpio_t *iface);

/* Even though there are more channels in EXTI, only 16 is used for pins */
static gpio_edge_state_t edge_state[16] = {
    0
};

/* Mapping for exti pin and line reference */
static const uint32_t exti_line[16] = {
    LL_SYSCFG_EXTI_LINE0,
    LL_SYSCFG_EXTI_LINE1,
    LL_SYSCFG_EXTI_LINE2,
    LL_SYSCFG_EXTI_LINE3,
    LL_SYSCFG_EXTI_LINE4,
    LL_SYSCFG_EXTI_LINE5,
    LL_SYSCFG_EXTI_LINE6,
    LL_SYSCFG_EXTI_LINE7,
    LL_SYSCFG_EXTI_LINE8,
    LL_SYSCFG_EXTI_LINE9,
    LL_SYSCFG_EXTI_LINE10,
    LL_SYSCFG_EXTI_LINE11,
    LL_SYSCFG_EXTI_LINE12,
    LL_SYSCFG_EXTI_LINE13,
    LL_SYSCFG_EXTI_LINE14,
    LL_SYSCFG_EXTI_LINE15
};

static const uint32_t exti_irqn[16] = {
    EXTI0_IRQn,
    EXTI1_IRQn,
    EXTI2_IRQn,
    EXTI3_IRQn,
    EXTI4_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn
};

PP_TYPE_DECL(
    gpio,
    PP_GPIO,
    GPIO_NUM_ARGS,
    gpio_init,
    sizeof(gpio_if_t));

int gpio_init(
    if_header_t *iface)
{
    gpio_if_t *gpio = (gpio_if_t *) iface;
    if_rs_t *rs = &gpio->header.header.rscs[0];

    gpio->header.configure = gpio_if_configure;
    gpio->header.set_value = gpio_if_set_value;
    gpio->header.get_value = gpio_if_get_value;

    gpio->pin_id = rs->decl->ref;
    return 0;
}

int gpio_if_configure(
    if_gpio_t *iface,
    const if_gpio_cf_t *config)
{
    gpio_if_t *gpio = (gpio_if_t *) iface;
    switch (config->dir) {
    case GPIO_DIR_IN:
        /* TODO: Set pullup/pulldown */
        gpio_configure_in(gpio->pin_id);
        break;

    case GPIO_DIR_OUT:
        gpio_configure_out(gpio->pin_id);
        break;
    }

    if (config->edge_callback != NULL) {
        /* Requeset edge interrupt. */
        uint32_t port = (gpio->pin_id >> 8) & 0x0f;
        uint32_t pin = gpio->pin_id & 0x0f;
        uint32_t trigger = 0;
        gpio_edge_state_t *st = &edge_state[pin];

        if (st->edge_callback != NULL) {
            /* Only one EXTI can be used per pin number */
            return -1;
        }

        st->edge_callback = config->edge_callback;
        st->edge_storage = config->edge_storage;

        switch (config->edge) {
        case GPIO_EDGE_NONE:
            trigger = LL_EXTI_TRIGGER_NONE;
            break;

        case GPIO_EDGE_RISING:
            trigger = LL_EXTI_TRIGGER_RISING;
            break;

        case GPIO_EDGE_FALLING:
            trigger = LL_EXTI_TRIGGER_FALLING;
            break;

        case GPIO_EDGE_BOTH:
            trigger = LL_EXTI_TRIGGER_RISING_FALLING;
            break;
        }

        /* Enable the interrupt */
        LL_EXTI_Init(&(LL_EXTI_InitTypeDef) {
            .Line_0_31 = 1 << pin,
            .LineCommand = ENABLE,
            .Mode = LL_EXTI_MODE_IT,
            .Trigger = trigger
        });
        LL_SYSCFG_SetEXTISource(port, exti_line[pin]);

        NVIC_SetPriority(exti_irqn[pin], EXTI_IRQ_PRIORITY);
        NVIC_EnableIRQ(exti_irqn[pin]);
    }
    return 0;
}

void gpio_if_set_value(
    if_gpio_t *iface,
    bool value)
{
    gpio_if_t *gpio = (gpio_if_t *) iface;
    gpio_set_value(gpio->pin_id, value);
}

bool gpio_if_get_value(
    if_gpio_t *iface)
{
    gpio_if_t *gpio = (gpio_if_t *) iface;
    return gpio_get_value(gpio->pin_id);
}

static void exti_irqhandler(
    void)
{
    uint32_t triggered = LL_EXTI_ReadFlag_0_31(0x0000ffff);
    LL_EXTI_ClearFlag_0_31(triggered);

    int i;
    for (i = 0; i < 16; i++) {
        if (triggered & (1 << i)) {
            edge_state[i].edge_callback(edge_state[i].edge_storage);
        }
    }
}

/* *INDENT-OFF* */
void EXTI0_IRQHandler(void)     { exti_irqhandler(); }
void EXTI1_IRQHandler(void)     { exti_irqhandler(); }
void EXTI2_IRQHandler(void)     { exti_irqhandler(); }
void EXTI3_IRQHandler(void)     { exti_irqhandler(); }
void EXTI4_IRQHandler(void)     { exti_irqhandler(); }
void EXTI9_5_IRQHandler(void)   { exti_irqhandler(); }
void EXTI15_10_IRQHandler(void) { exti_irqhandler(); }
/* *INDENT-ON* */
