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

typedef struct gpio_if_s gpio_if_t;

struct gpio_if_s {
    if_gpio_t header;
    uint32_t pin_id;
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
