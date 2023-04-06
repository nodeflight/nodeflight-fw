ifeq (1,$(includecount))

include make_conf/nodeflight-common.mk

INCLUDES += \
	platform/stm32 \
	vendor/stm32/cmsis_core/Core/Include

SOURCES += \
	platform/stm32/peripheral/gpio.c \
	platform/stm32/peripheral/spi.c \
	platform/stm32/peripheral/timer.c \
	platform/stm32/peripheral/uart.c \
	platform/stm32/peripheral/usb_vcp.c \
	platform/stm32/resource/dma.c \
	platform/stm32/resource/gpio.c \
	platform/stm32/hwmap/resource_stm32.c \
	platform/stm32/fault_irq.c

CFLAGS += \
	-DUSE_FULL_LL_DRIVER \
	-DHSE_VALUE=8000000

endif