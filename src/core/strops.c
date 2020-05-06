#include "core/strops.h"

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
    while(**ptr != ' ' && **ptr != '\0') {
        (*ptr)++;
    }

    while(**ptr == ' ') {
        (*ptr)++;
    }

    return start;
}