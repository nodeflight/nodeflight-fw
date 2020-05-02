#include <stdio.h>

#include "platform/platform.h"

int main(
    void)
{
    int i;
    platform_init();
    for (i = 0;; i++) {
        printf("Test %d %f\n", i, (double)4.34f);
    }
}
