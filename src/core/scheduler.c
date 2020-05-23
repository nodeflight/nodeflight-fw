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

#include "core/scheduler.h"
#include "lib/strops.h"
#include <stddef.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define SCHEDULER_CLIENTS_PER_POOL    32
#define SCHEDULER_TASK_BASE_PRIORITY  10
#define SCHEDULER_NUM_PRIORITIES      10
#define SCHEDULER_NOTIFY_EXECUTE      (1 << 0)
#define SCHEDULER_TIMEOUT             (1000 * portTICK_PERIOD_MS)

#define SCHEDULER_STACK_WORDS         1024

typedef struct sc_client_pool_s sc_client_pool_t;

struct sc_client_s {
    sc_clinet_init_t init;
    sc_clinet_run_t run;
    void *storage;
};

struct sc_client_pool_s {
    sc_client_pool_t *next;
    sc_client_t clients[SCHEDULER_CLIENTS_PER_POOL];
};

struct sc_s {
    sc_t *next;
    sc_client_pool_t *clients;
    char *name;
    TaskHandle_t task;
    volatile int clients_initialized;
    float period_sec;
};

static sc_t *schedulers;
SemaphoreHandle_t schedulers_mutex;

static void sc_task(
    void *storage);

sc_t *sc_get(
    const char *name)
{
    sc_t *cur;
    sc_t *sched;

    if (pdFALSE == xSemaphoreTake(schedulers_mutex, portMAX_DELAY)) {
        return NULL;
    }
    sched = NULL;
    for (cur = schedulers; cur != NULL && sched == NULL; cur = cur->next) {
        if (strops_word_cmp(cur->name, name) == 0) {
            sched = cur;
        }
    }

    xSemaphoreGive(schedulers_mutex);
    return sched;
}

void sc_init(
    void)
{
    schedulers = NULL;
    schedulers_mutex = xSemaphoreCreateMutex();
}

sc_client_t *sc_register_client(
    sc_t *sched,
    sc_clinet_init_t init,
    sc_clinet_run_t run,
    void *storage)
{
    int i;
    sc_client_pool_t *pool;
    sc_client_pool_t *last_pool;
    if (pdFALSE == xSemaphoreTake(schedulers_mutex, portMAX_DELAY)) {
        return NULL;
    }

    /* Add to existing pool, exit if successful */
    last_pool = NULL;
    for (pool = sched->clients; pool != NULL; pool = pool->next) {
        for (i = 0; i < SCHEDULER_CLIENTS_PER_POOL; i++) {
            sc_client_t *cli = &pool->clients[i];
            if (cli->init == NULL) {
                cli->init = init;
                cli->run = run;
                cli->storage = storage;
                xSemaphoreGive(schedulers_mutex);
                return cli;
            }
        }
        last_pool = pool;
    }

    /* No space left, add pool at end */
    pool = pvPortMalloc(sizeof(sc_client_pool_t));
    if (pool == NULL) {
        xSemaphoreGive(schedulers_mutex);
        return NULL;
    }
    for (i = 0; i < SCHEDULER_CLIENTS_PER_POOL; i++) {
        pool->clients[i].init = NULL;
        pool->clients[i].run = NULL;
        pool->clients[i].storage = NULL;
    }
    pool->next = NULL;
    pool->clients[0].init = init;
    pool->clients[0].run = run;
    pool->clients[0].storage = storage;

    /* Attach new pool at end */
    if (last_pool == NULL) {
        /* No pool in scheduler, add to scheduler */
        sched->clients = pool;
    } else {
        /* Pool is available, add to end */
        last_pool->next = pool;
    }

    xSemaphoreGive(schedulers_mutex);
    return &pool->clients[0];
}

sc_t *sc_define(
    const char *name,
    int priority)
{
    sc_t *cur;
    sc_t *sched;

    if (priority < 0 || priority >= SCHEDULER_NUM_PRIORITIES) {
        return NULL;
    }

    if (pdFALSE == xSemaphoreTake(schedulers_mutex, portMAX_DELAY)) {
        return NULL;
    }

    sched = NULL;

    /* Protect against concurrent double creation */
    for (cur = schedulers; cur != NULL && sched == NULL; cur = cur->next) {
        if (strops_word_cmp(cur->name, name) == 0) {
            xSemaphoreGive(schedulers_mutex);
            return NULL;
        }
    }

    sched = pvPortMalloc(sizeof(sc_t));
    if (sched == NULL) {
        xSemaphoreGive(schedulers_mutex);
        return NULL;
    }
    sched->clients = NULL;
    sched->name = strops_word_dup(name);
    if (sched->name == NULL) {
        xSemaphoreGive(schedulers_mutex);
        return NULL;
    }
    sched->clients_initialized = 0;
    sched->period_sec = -1.0f;
    sched->next = NULL;
    if (pdFAIL == xTaskCreate(sc_task,
        "sc_task",
        SCHEDULER_STACK_WORDS,
        sched,
        SCHEDULER_TASK_BASE_PRIORITY + priority,
        &sched->task)) {
        xSemaphoreGive(schedulers_mutex);
        return NULL;
    }

    sched->next = schedulers;
    schedulers = sched;

    xSemaphoreGive(schedulers_mutex);
    return sched;
}

int sc_configure_source(
    sc_t *sched,
    float period_sec)
{
    if (sched->period_sec >= 0.0f) {
        return -1;
    }
    sched->period_sec = period_sec;
    return 0;
}

void sc_trigger_from_isr(
    sc_t *sched)
{
    BaseType_t should_switch = pdFALSE;
    xTaskNotifyFromISR(sched->task, SCHEDULER_NOTIFY_EXECUTE, eSetBits, &should_switch);
    portYIELD_FROM_ISR(should_switch);
}

void sc_trigger(
    sc_t *sched)
{
    xTaskNotify(sched->task, SCHEDULER_NOTIFY_EXECUTE, eSetBits);
}

int sc_init_clients(
    void)
{
    sc_t *sched;
    sc_client_pool_t *pool;
    int cur_client;
    int i;

    if (pdFALSE == xSemaphoreTake(schedulers_mutex, portMAX_DELAY)) {
        return -1;
    }

    for (sched = schedulers; sched != NULL; sched = sched->next) {
        if (sched->period_sec < 0.0f) {
            xSemaphoreGive(schedulers_mutex);
            return -1;
        }
        cur_client = 0;
        for (pool = sched->clients; pool != NULL; pool = pool->next) {
            for (i = 0; i < SCHEDULER_CLIENTS_PER_POOL; i++) {
                if (cur_client >= sched->clients_initialized && pool->clients[i].init != NULL) {
                    pool->clients[i].init(sched->period_sec, pool->clients[i].storage);
                    sched->clients_initialized = cur_client + 1;
                }
                cur_client++;
            }
        }
    }

    xSemaphoreGive(schedulers_mutex);
    return 0;
}

void sc_task(
    void *storage)
{
    sc_t *sched = (sc_t *) storage;

    sc_client_pool_t *pool;
    int i;
    int clients_left;

    uint32_t notify_value;

    for (;;) {
        xTaskNotifyWait(0, UINT32_MAX, &notify_value, SCHEDULER_TIMEOUT);
        if (notify_value & SCHEDULER_NOTIFY_EXECUTE) {
            /*
             * Should not be an issue with races... Any registered client will always be registered, so
             * sched->clients_initialized will only increase. Therefore all clients reached is properly initialized
             */
            clients_left = sched->clients_initialized;
            for (pool = sched->clients; pool != NULL; pool = pool->next) {
                for (i = 0; i < SCHEDULER_CLIENTS_PER_POOL && clients_left > 0; i++) {
                    if (pool->clients[i].run != NULL) {
                        pool->clients[i].run(pool->clients[i].storage);
                    }
                    clients_left--;
                }
            }
        }
    }
}
