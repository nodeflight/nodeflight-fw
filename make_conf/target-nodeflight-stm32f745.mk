ifeq (1,$(includecount))

include make_conf/nodeflight-stm32f7xx.mk
include make_conf/modules.mk

CFLAGS += \
	-DSTM32F745xx

SOURCEDIRS += \
	platform/stm32f7xx/stm32f745

SOURCES += \
	vendor/stm32/cmsis_device_f7/Source/Templates/gcc/startup_stm32f745xx.s

LDFLAGS += \
	-Tstm32f745.ld \
	-Wl,-Lsrc/platform/stm32 \
	-Wl,-Lsrc/platform/stm32f7xx

endif