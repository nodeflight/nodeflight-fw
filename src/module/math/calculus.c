
#include "core/scheduler.h"
#include "core/module.h"
#include "core/variable.h"

#include "FreeRTOS.h"
#include "task.h"

typedef struct integrate_s integrate_t;

struct integrate_s {
    float *in;
    float out;
    float period_sec;
};

/* Common for all binary operations */

static void integrate_sch_init(
    float period_sec,
    void *storage)
{
    integrate_t *integrate = storage;
    integrate->period_sec = period_sec;
}
static void integrate_sch_run(
    void *storage)
{
    integrate_t *integrate = storage;
    integrate->out += integrate->period_sec * *integrate->in;
}
static int integrate_init(
    const char *name,
    md_arg_t *args)
{
    integrate_t *integrate = pvPortMalloc(sizeof(integrate_t));
    if (integrate == NULL) {
        return -1;
    }

    integrate->out = 0.0f;
    integrate->period_sec = 0.0f;
    vr_register(name, "f", &integrate->out);

    integrate->in = NULL;
    vr_request(args[1].name, 'f', (void **) &integrate->in);

    sc_register_client(args[0].sched, integrate_sch_init, integrate_sch_run, integrate);
    return 0;
}
MD_DECL(int_f, "sn", integrate_init);
