/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) Max Sikström
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

/*
 * The configuration of FreeRTOS for the native target, on the POSIX port.
 *
 * The same as the one of the STM32 chips, src/platform/stm32/FreeRTOSConfig.h,
 * except for what is of the hardware: no interrupt priorities, and the clock
 * is the host's.
 */

#pragma once

#include "systime.h"
#include <stdlib.h>

#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
/* Not known of the host, and not used by the POSIX port: reported as 0 MHz */
#define configCPU_CLOCK_HZ                      0
#define configTICK_RATE_HZ                      1000
#define configMAX_PRIORITIES                    16
#define configMINIMAL_STACK_SIZE                128
#define configMAX_TASK_NAME_LEN                 32
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           0
#define configUSE_ALTERNATIVE_API               0 /* Deprecated! */
#define configQUEUE_REGISTRY_SIZE               10
#define configUSE_QUEUE_SETS                    0
#define configUSE_TIME_SLICING                  0
#define configUSE_NEWLIB_REENTRANT              0
#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configSTACK_DEPTH_TYPE                  uint16_t
#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t

/* Thread local storage */
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 1
#define THREAD_LOCAL_STORAGE_TIMESTAT_IDX       0

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   10240
#define configAPPLICATION_ALLOCATED_HEAP        1

/* Hook function related definitions. */
/* Sleeps until the next tick, see platform.c */
#define configUSE_IDLE_HOOK                     1
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          1
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           1
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/*
 * The run time counter is systime_get_runtime(), as on the STM32 chips.
 * portmacro.h of the POSIX port defines portGET_RUN_TIME_COUNTER_VALUE() and
 * portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() itself, so the counter is given as
 * the alternative, which FreeRTOS uses first.
 */
#define portALT_GET_RUN_TIME_COUNTER_VALUE(dest) ((dest) = systime_get_runtime())

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               3
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE
#define configTIMER_SERVICE_TASK_NAME           "sys timer"
#define configIDLE_TASK_NAME                    "sys idle"

/* Define to trap errors during development. */
#define configASSERT(x)                         if ( (x) == 0) abort()

/* FreeRTOS MPU specific definitions. */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

#define INCLUDE_vTaskDelay                      1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskSuspend                    1
