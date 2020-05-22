#include "FreeRTOS.h"
#include "task.h"

void vApplicationStackOverflowHook(
    TaskHandle_t *pxTask,
    signed portCHAR *pcTaskName)
{
    asm ("bkpt 255");
    for (;;);
}
