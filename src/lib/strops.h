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

#include <stdint.h>

int strops_cmp(
    const char *word,
    const char *line);

int strops_line_copy(
    char *dst,
    int dst_len,
    const char **ptr);

char *strops_dup(
    const char *source);

int32_t strops_word_to_int(
    const char *word);

float strops_word_to_float(
    const char *line);

char **strops_argv_dup(
    char *const *argv);

int strops_get_argc(
    char *const *argv);

/* TODO: limit argv length */
int strops_split_argv(
    char *src,
    char **argv);
