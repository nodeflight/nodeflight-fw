ifeq (1,$(includecount))

include make_conf/nodeflight-stm32f4xx.mk
include make_conf/modules.mk
	
CFLAGS += -DSTM32F405xx

SOURCEDIRS += \
	platform/stm32f4xx/stm32f405

SOURCES += \
	vendor/stm32/cmsis_device_f4/Source/Templates/gcc/startup_stm32f405xx.s

LDFLAGS += \
	-Tstm32f405.ld \
	-Wl,-Lsrc/platform/stm32 \
	-Wl,-Lsrc/platform/stm32f4xx

endif