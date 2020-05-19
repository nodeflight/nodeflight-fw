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

#include "core/interface.h"

typedef struct if_serial_s if_serial_t;
typedef struct if_serial_config_s if_serial_config_t;

#define IF_SERIAL_INVERTED_TX 0x00000001
#define IF_SERIAL_INVERTED_RX 0x00000002
#define IF_SERIAL_HALF_DUPLEX 0x00000004

struct if_serial_config_s {
    uint32_t baudrate;
    uint16_t tx_buf_size;
    uint16_t rx_buf_size;
    uint32_t flags;

    void (*rx_done)(
        const uint8_t *buf,
        uint16_t len,
        void *storage);

    void *storage;
};

struct if_serial_s {
    if_header_t header;

    int (*configure)(
        if_serial_t *iface,
        const if_serial_config_t *config);

    int (*tx_write)(
        if_serial_t *iface,
        const void *buf,
        int bytes);

    void (*tx_wait_done)(
        if_serial_t *iface);
};

#define IF_SERIAL(_INTERFACE) ((if_serial_t *) (_INTERFACE))

typedef struct if_pwm_s if_pwm_t;
typedef struct if_pwm_config_s if_pwm_config_t;

struct if_pwm_config_s {
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
     * Number of pulses in a cycle.
     *
     * For servo output, only single pulse width is repeated, and therefore num_pulses=1
     *
     * For DSHOT, margin is needed between bursts of 16 pulses. Reasonable value would be 20-25
     */
    uint16_t pulses_cycle;
};

struct if_pwm_s {
    if_header_t header;

    /**
     * Configure PWM output
     */
    int (*configure)(
        if_pwm_t *iface,
        const if_pwm_config_t *config);

    /**
     * Set output
     *
     * pulse_widths points to an array of the configured size of pulses_count
     */
    int (*set_output)(
        if_pwm_t *iface,
        const uint16_t *pulse_widths);
};

#define IF_PWM(_INTERFACE) ((if_pwm_t *) (_INTERFACE))
