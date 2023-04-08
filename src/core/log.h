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

#include "FreeRTOS.h"
#include "task.h"

#define LOG_MAX_LEN           80

typedef struct log_line_s log_line_t;

typedef void (*log_handler_t)(
    TaskHandle_t task,
    TickType_t tick,
    const char *message,
    void *storage);

struct log_line_s {
    TaskHandle_t task;
    TickType_t tick;
    int len;
    char buffer[LOG_MAX_LEN + 1];
};

void log_init(
    void);

void log_register(
    log_handler_t handler,
    void *storage);

void log_println(
    const char *fmt,
    ...);

void log_begin_line(
    log_line_t *log_line);

void log_printf(
    log_line_t *log_line,
    const char *fmt,
    ...);

void log_finish_line(
    log_line_t *log_line);
