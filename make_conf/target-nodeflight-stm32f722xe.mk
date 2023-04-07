ifeq (1,$(includecount))

include make_conf/nodeflight-stm32f7xx.mk
include make_conf/modules.mk

CFLAGS += \
	-DSTM32F722xx

SOURCEDIRS += \
	platform/stm32f7xx/stm32f722

SOURCES += \
	vendor/stm32/cmsis_device_f7/Source/Templates/gcc/startup_stm32f722xx.s

LDFLAGS += \
	-Wl,-Llinker \
	-TSTM32F722xE.ld

endif