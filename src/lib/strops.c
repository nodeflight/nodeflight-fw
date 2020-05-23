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

#include "lib/strops.h"
#include <stddef.h>
#include <stdbool.h>
#include "FreeRTOS.h"

/* TODO: unit test */
int strops_word_cmp(
    const char *word,
    const char *line)
{
    while (*word != '\0' && *line != '\0' && *line != ' ') {
        if (*word < *line) {
            return -1;
        }
        if (*word > *line) {
            return 1;
        }
        word++;
        line++;
    }
    if (*word == '\0' && (*line != '\0' && *line != ' ')) {
        return -1;
    }
    if (*word != '\0' && (*line == '\0' || *line == ' ')) {
        return 1;
    }
    return 0;
}

/* TODO: unit test */
const char *strops_next_word(
    const char **ptr)
{
    const char *start = *ptr;
    while (**ptr != ' ' && **ptr != '\0') {
        (*ptr)++;
    }

    while (**ptr == ' ') {
        (*ptr)++;
    }

    return start;
}

/* TODO: unit test */
int strops_line_copy(
    char *dst,
    int dst_len,
    const char **ptr)
{
    int status = 0;

    /* Always keep space for termination */
    dst_len--;

    while (**ptr != '\n' && **ptr != '\0') {
        if (dst_len > 0) {
            *dst = **ptr;
            dst++;
            dst_len--;
        } else {
            status = -1;
        }
        (*ptr)++;
    }
    *dst = '\0';

    /* Skip newline, to start on next line next time */
    if (**ptr == '\n') {
        (*ptr)++;
    }

    return status;
}

char *strops_word_dup(
    const char *source)
{
    int len;
    int i;
    char *dest;

    len = 0;
    while (source[len] != '\0' && source[len] != '\n' && source[len] != ' ') {
        len++;
    }
    dest = pvPortMalloc(len + 1);
    if (dest == NULL) {
        return NULL;
    }
    for (i = 0; i < len; i++) {
        dest[i] = source[i];
    }
    dest[len] = '\0';
    return dest;
}

char *strops_line_dup(
    const char *source)
{
    int len;
    int i;
    char *dest;

    len = 0;
    while (source[len] != '\0' && source[len] != '\n') {
        len++;
    }
    dest = pvPortMalloc(len + 1);
    if (dest == NULL) {
        return NULL;
    }
    for (i = 0; i < len; i++) {
        dest[i] = source[i];
    }
    dest[len] = '\0';
    return dest;
}

int32_t strops_word_to_int(
    const char *word)
{
    bool neg = false;
    int32_t res = 0;

    if (*word == '-') {
        word++;
        neg = true;
    }

    while (*word >= '0' && *word <= '9') {
        res = (res * 10L) + (*word - '0');
        word++;
    }

    if (neg) {
        return -res;
    } else {
        return res;
    }
}

float strops_word_to_float(
    const char *word)
{
    bool neg = false;
    float res = 0.0f;

    if (*word == '-') {
        word++;
        neg = true;
    }

    while (*word >= '0' && *word <= '9') {
        res = (res * 10.0f) + (*word - '0');
        word++;
    }

    if (*word == '.') {
        word++;
        float fact = 0.1f;
        while (*word >= '0' && *word <= '9') {
            res += fact * (*word - '0');
            word++;
            fact *= 0.1f;
        }
    }

    if (neg) {
        return -res;
    } else {
        return res;
    }
}
