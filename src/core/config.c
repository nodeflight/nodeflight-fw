#include "core/config.h"
#include "core/strops.h"
#include "core/module.h"

#define LINEBUF_SIZE 256

extern const char __l1conf_start[];
static char config_linebuf[LINEBUF_SIZE];

static void config_parse_line(
    const char *line)
{
    const char *command = strops_next_word(&line);
    if (0 == strops_word_cmp("mod", command)) {
        /* Load module */
        const char *name = strops_next_word(&line);
        if (0 != module_init(name, line)) {
            /* TODO: Error handling */
        }
    } else if (0 == strops_word_cmp("per", command)) {
        /* Load peripheral definition */
    } else {
        /* TODO: Error handling */
    }
}

void config_init(
    void)
{
    const char *cur = __l1conf_start;
    while (*cur != '\0') {
        if (0 == strops_line_copy(config_linebuf, LINEBUF_SIZE, &cur)) {
            config_parse_line(config_linebuf);
        }
    }
}