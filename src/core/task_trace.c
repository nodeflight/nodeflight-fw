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

#include "core/task_trace.h"
#include "FreeRTOS.h"
#include "task.h"
#include "core/log.h"
#include "systime.h"
#include <string.h>
#include <stdlib.h>

#define TASK_TRACE_NUM_TASKS 32

#if configUSE_TRACE_FACILITY

static const char *task_state_name[] = {
    [eRunning] = "running",
    [eReady] = "ready",
    [eBlocked] = "blocked",
    [eSuspended] = "suspended",
    [eDeleted] = "deleted",
    [eInvalid] = "invalid"
};
typedef struct {
    configRUN_TIME_COUNTER_TYPE runtime_total_last;
    configRUN_TIME_COUNTER_TYPE runtime_since_last;
} task_trace_task_info_t;

static TaskStatus_t *task_trace_task_status;
static TaskStatus_t **task_trace_task_sorted_status;
static configRUN_TIME_COUNTER_TYPE task_trace_last_runtime;

void task_trace_init(
    void)
{
    task_trace_task_status = pvPortMalloc(sizeof(TaskStatus_t) * TASK_TRACE_NUM_TASKS);
    task_trace_task_sorted_status = pvPortMalloc(sizeof(TaskStatus_t *) * TASK_TRACE_NUM_TASKS);
    task_trace_last_runtime = systime_get_runtime();
}

static int task_trace_cmp(
    const void *ela,
    const void *elb)
{
    task_trace_task_info_t *tia;
    task_trace_task_info_t *tib;
    tia = pvTaskGetThreadLocalStoragePointer(
        (*(TaskStatus_t **) ela)->xHandle,
        THREAD_LOCAL_STORAGE_TIMESTAT_IDX
    );
    tib = pvTaskGetThreadLocalStoragePointer(
        (*(TaskStatus_t **) elb)->xHandle,
        THREAD_LOCAL_STORAGE_TIMESTAT_IDX
    );
    if (tia->runtime_since_last < tib->runtime_since_last) {
        return 1;
    } else if (tia->runtime_since_last > tib->runtime_since_last) {
        return -1;
    } else {
        return 0;
    }
}

void task_trace_print(
    void)
{
    UBaseType_t num_tasks;
    configRUN_TIME_COUNTER_TYPE runtime;
    configRUN_TIME_COUNTER_TYPE tot_since_last;
    UBaseType_t i;

    num_tasks = uxTaskGetSystemState(task_trace_task_status, 32, &runtime);

    tot_since_last = runtime - task_trace_last_runtime;
    task_trace_last_runtime = runtime;

    log_println("");
    log_println("num_tasks = %lu", num_tasks);
    for (i = 0; i < num_tasks; i++) {
        TaskStatus_t *t = &task_trace_task_status[i];

        task_trace_task_info_t *ti;
        ti = pvTaskGetThreadLocalStoragePointer(t->xHandle, THREAD_LOCAL_STORAGE_TIMESTAT_IDX);
        if (ti == NULL) {
            ti = pvPortMalloc(sizeof(task_trace_task_info_t));
            memset(ti, 0, sizeof(task_trace_task_info_t));
            vTaskSetThreadLocalStoragePointer(t->xHandle, THREAD_LOCAL_STORAGE_TIMESTAT_IDX, ti);
        }

        ti->runtime_since_last = t->ulRunTimeCounter - ti->runtime_total_last;
        ti->runtime_total_last = t->ulRunTimeCounter;

        task_trace_task_sorted_status[i] = t;
    }

    qsort(
        task_trace_task_sorted_status,
        num_tasks,
        sizeof(task_trace_task_info_t *),
        task_trace_cmp
    );

    log_println("ID ................name ....state time ...prio free");
    for (i = 0; i < num_tasks; i++) {
        TaskStatus_t *t = task_trace_task_sorted_status[i];
        task_trace_task_info_t *ti;
        ti = pvTaskGetThreadLocalStoragePointer(t->xHandle, THREAD_LOCAL_STORAGE_TIMESTAT_IDX);
        log_println("%2lu %-20s %9s %3d%% %2lu (%2lu) %4u",
            t->xTaskNumber,
            t->pcTaskName,
            task_state_name[t->eCurrentState],
            (int) (100.0 * (float) ti->runtime_since_last / (float) tot_since_last),
            t->uxCurrentPriority,
            t->uxBasePriority,
            t->usStackHighWaterMark
        );
    }
}
#else

void task_trace_init(
    void)
{

}

void task_trace_print(
    void)
{

}
#endif
