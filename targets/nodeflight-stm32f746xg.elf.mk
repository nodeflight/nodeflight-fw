
include vendor/stm32/stm32f746xg.mk
include vendor/freertos.mk
include vendor/fatfs.mk
include vendor/tinyprintf.mk
include make/nodeflight-stm32f7xx.mk

STM32_MODEL=stm32f746

CFLAGS += \
	-DSTM32F746xx

LDFLAGS += \
	-Wl,-Llinker \
	-TSTM32F746xG.ld

INC += \
	src/integration/fatfs \
	src/platform/stm32 \
	src/platform/stm32f7xx \
	src

CFLAGS += \
	-DUSE_FULL_LL_DRIVER \
	-DHSE_VALUE=8000000

include $(CBE)/lang/gcc/appl.mk