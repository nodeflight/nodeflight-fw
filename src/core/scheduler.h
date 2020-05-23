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

typedef struct sc_s sc_t;
typedef struct sc_client_s sc_client_t;

typedef void (*sc_clinet_init_t)(
    float period_sec,
    void *storage);

typedef void (*sc_clinet_run_t)(
    void *storage);

void sc_init(
    void);

sc_t *sc_define(
    const char *name,
    int priority);

sc_client_t *sc_register_client(
    sc_t *sched,
    sc_clinet_init_t init,
    sc_clinet_run_t run,
    void *storage);

sc_t *sc_get(
    const char *name);

int sc_configure_source(
    sc_t *sched,
    float period_sec);

void sc_trigger_from_isr(
    sc_t *scheduler);

void sc_trigger(
    sc_t *scheduler);

int sc_init_clients(
    void);
