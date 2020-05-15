#include "core/config.h"
#include "core/module.h"
#include "lib/strops.h"
#include "lib/map.h"

#define LINEBUF_SIZE 256

extern const char __l1conf_start[];

static char config_linebuf[LINEBUF_SIZE];
static map_t *config_per;

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
        const char *name = strops_next_word(&line);
        map_set(config_per, name, strops_line_dup(line));
    } else {
        /* TODO: Error handling */
    }
}

void config_init(
    void)
{
    const char *cur = __l1conf_start;

    config_per = map_create();

    while (*cur != '\0') {
        if (0 == strops_line_copy(config_linebuf, LINEBUF_SIZE, &cur)) {
            config_parse_line(config_linebuf);
        }
    }
}

const char *config_get_peripheral_config(
    const char *tag)
{
    return map_get(config_per, tag);
}