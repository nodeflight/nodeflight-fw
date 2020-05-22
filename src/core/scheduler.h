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

typedef struct scheduler_s scheduler_t;
typedef struct scheduler_client_s scheduler_client_t;

typedef void (*scheduler_clinet_init_t)(
    float period_sec,
    void *storage);

typedef void (*scheduler_clinet_run_t)(
    void *storage);

void scheduler_init(
    void);

scheduler_t *scheduler_define(
    const char *name,
    int priority);

scheduler_client_t *scheduler_register_client(
    scheduler_t *sched,
    scheduler_clinet_init_t init,
    scheduler_clinet_run_t run,
    void *storage);

scheduler_t *scheduler_get(
    const char *name);

int scheduler_configure_source(
    scheduler_t *sched,
    float period_sec);

void scheduler_trigger_from_isr(
    scheduler_t *scheduler);

void scheduler_trigger(
    scheduler_t *scheduler);

int scheduler_init_clients(
    void);
