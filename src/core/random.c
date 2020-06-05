#include "platform/platform.h"
#include "core/random.h"
#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t random_mutex;

void random_init(
    void)
{
    random_mutex = xSemaphoreCreateMutex();
}

uint32_t random_get(
    void)
{
    uint32_t random = 0;
    if (pdFALSE == xSemaphoreTake(random_mutex, portMAX_DELAY)) {
        /* Should never happen */
        return 0;
    }
    random = platform_random_get();
    xSemaphoreGive(random_mutex);
    return random;
}
