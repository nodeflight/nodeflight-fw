#pragma once

int strops_word_cmp(
    const char *word,
    const char *line);

const char *strops_next_word(
    const char **ptr);

int strops_line_copy(
    char *dst,
    int dst_len,
    const char **ptr);