#include <stdarg.h>

#include "core/log.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "vendor/tinyprintf/tinyprintf.h"

#define LOG_QUEUE_LEN          8

#define LOG_STACK_WORDS      128
#define LOG_TASK_PRIORITY      2

typedef struct log_handler_node_s log_handler_node_t;

struct log_handler_node_s {
    log_handler_node_t *next;
    log_handler_t handler;
    void *storage;
};

static void log_task(
    void *storage);

static log_handler_node_t *log_handlers;
static QueueHandle_t log_queue;

void log_init(
    void)
{
    log_handlers = NULL;

    log_queue = xQueueCreate(LOG_QUEUE_LEN, sizeof(log_line_t));

    xTaskCreate(log_task,
        "sys log",
        LOG_STACK_WORDS,
        NULL,
        LOG_TASK_PRIORITY,
        NULL);
}

void log_register(
    log_handler_t handler,
    void *storage)
{
    log_handler_node_t *new_handler;
    new_handler = pvPortMalloc(sizeof(log_handler_node_t));
    if (new_handler == NULL) {
        return;
    }
    new_handler->handler = handler;
    new_handler->storage = storage;

    /* Don't make it more complicated that it is for a simple task like swapping a pointer */
    vTaskSuspendAll();
    new_handler->next = log_handlers;
    log_handlers = new_handler;
    xTaskResumeAll();
}

void log_println(
    const char *fmt,
    ...)
{
    log_line_t log_line;
    va_list ap;
    log_line.task = xTaskGetCurrentTaskHandle();

    va_start(ap, fmt);
    tfp_vsnprintf(log_line.buffer, LOG_MAX_LEN, fmt, ap);
    va_end(ap);

    xQueueSendToBack(log_queue, &log_line, portMAX_DELAY);
}

void log_begin_line(
    log_line_t *log_line)
{
    log_line->task = xTaskGetCurrentTaskHandle();
    log_line->len = 0;
}

void log_printf(
    log_line_t *log_line,
    const char *fmt,
    ...)
{
    va_list ap;
    int len;

    if (log_line->len >= LOG_MAX_LEN) {
        return;
    }

    va_start(ap, fmt);
    len = tfp_vsnprintf(log_line->buffer + log_line->len, LOG_MAX_LEN - log_line->len + 1, fmt, ap);
    va_end(ap);

    if (len > LOG_MAX_LEN - log_line->len) {
        len = LOG_MAX_LEN - log_line->len;
    }
    log_line->len += len;
}

void log_finish_line(
    log_line_t *log_line)
{
    xQueueSendToBack(log_queue, log_line, portMAX_DELAY);
}

static void log_task(
    void *storage)
{
    log_line_t log_line;
    log_handler_node_t *handler;
    for (;;) {
        if (pdTRUE == xQueueReceive(log_queue, &log_line, portMAX_DELAY)) {
            for (handler = log_handlers; handler != NULL; handler = handler->next) {
                handler->handler(log_line.task, log_line.buffer, handler->storage);
            }
        }
    }
}
