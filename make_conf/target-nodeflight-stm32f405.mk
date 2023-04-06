ifeq (1,$(includecount))

include make_conf/nodeflight-stm32f4xx.mk
include make_conf/modules.mk

SOURCES += \
	vendor/stm32/cmsis_device_f4/Source/Templates/gcc/startup_stm32f405xx.s \
	platform/stm32f4xx/hwmap_stm32f405.c
	
CFLAGS += -DSTM32F405xx

LDFLAGS += \
	-Tstm32f405.ld \
	-Wl,-Lsrc/platform/stm32 \
	-Wl,-Lsrc/platform/stm32f4xx

endif