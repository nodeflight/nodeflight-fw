ifeq (1,$(includecount))

include make_conf/nodeflight-common.mk

SOURCEDIRS += \
	platform/stm32 \
	platform/stm32/peripheral \
	platform/stm32/resource \
	platform/stm32/hwmap

INCLUDES += \
	platform/stm32 \
	vendor/stm32/cmsis_core/Core/Include

CFLAGS += \
	-DUSE_FULL_LL_DRIVER \
	-DHSE_VALUE=8000000

endif