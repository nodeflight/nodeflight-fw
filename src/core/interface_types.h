/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2023  Max Sikström
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

#include "core/interface.h"
#include "FreeRTOS.h"
#include <stdbool.h>

typedef struct if_gpio_s if_gpio_t;
typedef struct if_gpio_cf_s if_gpio_cf_t;

typedef enum if_gpio_dir_s {
    GPIO_DIR_IN = 0,
    GPIO_DIR_OUT
} if_gpio_dir_t;

typedef enum if_gpio_pull_s {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN
} if_gpio_pull_t;

typedef enum if_gpio_edge_s {
    GPIO_EDGE_NONE = 0,
    GPIO_EDGE_RISING,
    GPIO_EDGE_FALLING,
    GPIO_EDGE_BOTH
} if_gpio_edge_t;

struct if_gpio_cf_s {
    /**
     * GPIO pin direction
     */
    if_gpio_dir_t dir;

    /**
     * GPIO pin pull resistor
     */
    if_gpio_pull_t pull;

    /**
     * Pin change interrupt config
     */
    if_gpio_edge_t edge;

    /**
     * Pin change interrupt routine
     */
    void (*edge_callback)(
        void *storage);

    /**
     * Pin change interrupt storage
     */
    void *edge_storage;
};

struct if_gpio_s {
    if_header_t header;

    int (*configure)(
        if_gpio_t *iface,
        const if_gpio_cf_t *config);

    void (*set_value)(
        if_gpio_t *iface,
        bool value);

    bool (*get_value)(
        if_gpio_t *iface);
};

#define IF_GPIO(_INTERFACE) ((if_gpio_t *) (_INTERFACE))

typedef struct if_serial_s if_serial_t;
typedef struct if_serial_cf_s if_serial_cf_t;

/**
 * The TX line is inverted, which means active low.
 */
#define IF_SERIAL_INVERTED_TX         0x00000001

/**
 * The RX line is inverted, which means active low.
 */
#define IF_SERIAL_INVERTED_RX         0x00000002

/**
 * RX is coupled to the TX line, and TX is in tri-state when not transmitting
 */
#define IF_SERIAL_HALF_DUPLEX         0x00000004

/**
 * The protocol has a frame delimiter character
 *
 * In case of having a frame delinmiter, two things may happen:
 *
 * 1. the serial backend may use the delimiter to hint when to wake up the task for further processing, to reduce the
 *    number of task switches. It is not required however.
 * 2. The backend must stop rx_read operation on the frame delimiter, so if a frame delimiter is received, the rx_read
 *    operation is interrupted and data is returned, including the frame delimiter. This makes it possible to assume
 *    a frame start always occurs at first byte in a buffer if previous buffer ended with the frame delimiter.
 *
 * Useful for protocols similar to HDLC, to reduce the amount of task switching, or for line buffered input with frame
 * delimiter set to '\n'
 *
 * If set, also set the frame_delimiter parameter in the configuration struct.
 */
#define IF_SERIAL_HAS_FRAME_DELIMITER 0x00000008

struct if_serial_cf_s {
    uint32_t baudrate;
    uint16_t tx_buf_size;
    uint16_t rx_buf_size;
    uint32_t flags;

    uint8_t frame_delimiter;

    void *storage;
};

struct if_serial_s {
    if_header_t header;

    int (*configure)(
        if_serial_t *iface,
        const if_serial_cf_t *config);

    int (*tx_write)(
        if_serial_t *iface,
        const void *buf,
        int bytes);

    void (*tx_wait_done)(
        if_serial_t *iface,
        TickType_t timeout);

    int (*rx_read)(
        if_serial_t *iface,
        uint8_t *dst,
        int dst_size,
        TickType_t timeout);
};

#define IF_SERIAL(_INTERFACE) ((if_serial_t *) (_INTERFACE))

typedef struct if_pwm_s if_pwm_t;
typedef struct if_pwm_cf_s if_pwm_cf_t;

struct if_pwm_cf_s {
    /**
     * Clock frequency
     *
     * The clock speed sets the resolution of the timer, as updates can only occur on clock ticks.
     *
     * For common servo output of 1-2ms pulse, 1000 steps, needs 1us resolution. Therefore frequency needs to be
     * 1/1us = 1000000Hz
     *
     * For DSHOT600 with 600kbits/second, and one bit is a pulse either of 33% on, 67% off or 67%off and 33% on,
     * gives a resolution of three ticks per bit, thus 1800000 Hz
     */
    uint32_t clock_hz;     /**< clock-rate. For 1000 steps 1-2ms 50Hz pulses, 1MHz clock is required */

    /**
     * Number of ticks per pulse
     *
     * Sets the rate for how many ticks before restarting a pulse.
     *
     * For common servo output of 50Hz pulses and 1-2ms, based on a clock of 1Mhz, there is 20ms between pulses.
     * Therefore 20000 ticks between pulses. The pulse width value would then be in the interval of 1000-2000
     */
    uint32_t period_ticks;

    /**
     * Number of pulses in a burst.
     *
     * For servo output, only single pulse width is repeated, and therefore num_pulses=1
     *
     * For DSHOT, one bit is coded per pulse and 16 bits: Therefore num_pulses=16
     */
    uint16_t pulses_count;

    /**
     * Callback for updating pulse widths
     *
     * The function is called once every cycle, and is intended for fulling out one buffer.
     *
     * @param values array of pulse widths. Size specified in pulses_count
     */
    void (*update_values_cb)(
        uint32_t *values,
        void *storage);

    /**
     * Pointer to pass to the callbacks;
     */
    void *storage;
};

struct if_pwm_s {
    if_header_t header;

    /**
     * Configure PWM output
     */
    int (*configure)(
        if_pwm_t *iface,
        const if_pwm_cf_t *config);
};

#define IF_PWM(_INTERFACE) ((if_pwm_t *) (_INTERFACE))

typedef struct if_spi_s if_spi_t;
typedef struct if_spi_cf_s if_spi_cf_t;

typedef enum if_spi_mode_s {
    SPI_MODE_LEADING_HIGH = 0,
    SPI_MODE_LEADING_LOW,
    SPI_MODE_TRAILING_HIGH,
    SPI_MODE_TRAILING_LOW
} if_spi_mode_t;

struct if_spi_cf_s {
    /**
     * Maximum allowed baud rate
     *
     * The baudrate will be determined by the current clock setup and hardware abilities, but will never exceed
     * specified baud rate
     *
     * Unit in Hertz
     */
    uint32_t max_baud_rate;

    /**
     * SPI mode
     *
     * Sampling can either be done on leading or trailing edge of clock. Clock can either be active high or low.
     */
    if_spi_mode_t mode;
};

struct if_spi_s {
    if_header_t header;

    /**
     * Configure SPI output
     */
    int (*configure)(
        if_spi_t *iface,
        const if_spi_cf_t *config);

    /**
     * Transfer single byte syncrhonously
     */
    void (*transfer)(
        if_spi_t *iface,
        uint8_t *tx_buf,
        uint8_t *rx_buf,
        int length);
};

#define IF_SPI(_INTERFACE) ((if_spi_t *) (_INTERFACE))
