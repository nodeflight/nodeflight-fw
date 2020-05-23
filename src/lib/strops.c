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
int strops_cmp(
    const char *stra,
    const char *strb)
{
    while (*stra != '\0' && *strb != '\0') {
        if (*stra < *strb) {
            return -1;
        }
        if (*stra > *strb) {
            return 1;
        }
        stra++;
        strb++;
    }
    if (*stra == '\0' && *strb != '\0') {
        return -1;
    }
    if (*stra != '\0' && *strb == '\0') {
        return 1;
    }
    return 0;
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

char *strops_dup(
    const char *source)
{
    int len;
    int i;
    char *dest;

    len = 0;
    while (source[len] != '\0') {
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

char **strops_argv_dup(
    char *const *argv)
{
    const char *cur_char;
    int argc = 0;
    int buflen = 0;

    /* Calculate memory usage */
    argc = 0;
    buflen = 0;
    while (argv[argc] != NULL) {
        for (cur_char = argv[argc]; *cur_char != '\0'; cur_char++) {
            buflen++;
        }
        buflen++; /* null byte */
        argc++;
    }

    /* Allocate */
    char **out_argv;
    char *out_buf;

    out_argv = pvPortMalloc(sizeof(char *) * (argc + 1));
    out_buf = pvPortMalloc(sizeof(char) * buflen);

    /* Copy */
    argc = 0;
    buflen = 0;
    while (argv[argc] != NULL) {
        out_argv[argc] = &out_buf[buflen];
        for (cur_char = argv[argc]; *cur_char != '\0'; cur_char++) {
            out_buf[buflen++] = *cur_char;
        }
        out_buf[buflen++] = '\0';
        argc++;
    }
    out_argv[argc] = NULL;

    return out_argv;
}

int strops_get_argc(
    char *const *argv)
{
    int argc = 0;
    while (argv[argc] != NULL) {
        argc++;
    }
    return argc;
}

int strops_split_argv(
    char *src,
    char **argv)
{
    char *in_ptr = src;
    char *out_ptr = src;
    int argc = 0;

    while (*in_ptr != '\n' && *in_ptr != '\0') {
        /* Locate start of argument */
        while (*in_ptr == ' ') {
            in_ptr++;
        }

        /* Exit if end of string */
        if (*in_ptr == '\n' || *in_ptr == '\0') {
            break;
        }

        /* Is the argument quoted? */
        if (*in_ptr == '"') {
            /* Quoted string */
            in_ptr++;

            /* Copy and unescape argument content */
            argv[argc++] = out_ptr;
            while (*in_ptr != '"' && *in_ptr != '\0') {
                /* TODO: unescape */
                *(out_ptr++) = *(in_ptr++);
            }
            if (*in_ptr != '\0') {
                in_ptr++;
            }
            *(out_ptr)++ = '\0';
        } else {
            /* Not quited string */

            /* Copy argument content */
            argv[argc++] = out_ptr;
            while (*in_ptr != ' ' && *in_ptr != '\n' && *in_ptr != '\0') {
                *(out_ptr++) = *(in_ptr++);
            }
            if (*in_ptr != '\0') {
                /* Make sure to skip null-byte written by out_ptr on next line, if overlaps */
                in_ptr++;
            }
            *(out_ptr)++ = '\0';
        }
    }
    argv[argc] = NULL;
    return argc;
}
