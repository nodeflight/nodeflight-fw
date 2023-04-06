ifeq (1,$(includecount))

include make_conf/nodeflight-stm32f7xx.mk
include make_conf/modules.mk

SOURCES += \
	vendor/stm32/cmsis_device_f7/Source/Templates/gcc/startup_stm32f722xx.s \
	platform/stm32f7xx/hwmap_stm32f722.c

CFLAGS += \
	-DSTM32F722xx

LDFLAGS += \
	-Tstm32f722.ld \
	-Wl,-Lsrc/platform/stm32 \
	-Wl,-Lsrc/platform/stm32f7xx

endif