ifeq (1,$(includecount))

include make/nodeflight-common.mk

SRC += \
	src/platform/stm32/hwmap/resource_stm32.c          \
	src/platform/stm32/peripheral/gpio.c               \
	src/platform/stm32/peripheral/spi.c                \
	src/platform/stm32/peripheral/timer.c              \
	src/platform/stm32/peripheral/uart.c               \
	src/platform/stm32/peripheral/usb_vcp.c            \
	src/platform/stm32/resource/dma.c                  \
	src/platform/stm32/resource/gpio.c                 \
	src/platform/stm32/fault_irq.c

CFLAGS += \
	-DUSE_FULL_LL_DRIVER

endif