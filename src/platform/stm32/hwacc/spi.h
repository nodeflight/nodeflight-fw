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

#pragma once

#include <stdint.h>
#include "stm32.h"

typedef struct spi_def_s spi_def_t;

PP_TYPE_EXTERN(spi);

struct spi_def_s {
    SPI_TypeDef *reg;
    uint16_t id;
    int16_t IRQn;
};

#define SPI_DEF(_ID) \
    (void *) &(const spi_def_t) { \
        .reg = SPI ## _ID, \
        .id = _ID - 1, \
        .IRQn = SPI ## _ID ## _IRQn \
    }

enum {
    SPI_ARG_PIN_SCK = 0,
    SPI_ARG_PIN_MOSI,
    SPI_ARG_PIN_MISO,
    SPI_ARG_DMA_TX,
    SPI_ARG_DMA_RX,
    SPI_NUM_ARGS
};
