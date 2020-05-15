#include "lib/strops.h"
#include <stddef.h>
#include <stdlib.h>

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
    dest = malloc(len + 1);
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
    dest = malloc(len + 1);
    if (dest == NULL) {
        return NULL;
    }
    for (i = 0; i < len; i++) {
        dest[i] = source[i];
    }
    dest[len] = '\0';
    return dest;
}