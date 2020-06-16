#pragma once

#include "FreeRTOS.h"
#include "task.h"

#define LOG_MAX_LEN           80

typedef struct log_line_s log_line_t;

typedef void (*log_handler_t)(
    TaskHandle_t task,
    const char *message,
    void *storage);

struct log_line_s {
    TaskHandle_t task;
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
