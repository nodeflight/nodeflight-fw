ifeq (1,$(includecount))

include make/nodeflight-stm32-common.mk

FREERTOS_PORT = GCC/ARM_CM4F

SRC += \
	src/platform/stm32f4xx/platform.c \
	src/platform/stm32f4xx/systime.c \
	src/platform/stm32f4xx/$(STM32_MODEL)/hwmap_$(STM32_MODEL).c

SRC += \
	$(CMSIS_DEVICE_DIR)/Source/Templates/system_stm32f4xx.c \
	$(STM32_HAL_DIR)/Src/stm32f4xx_ll_dma.c \
	$(STM32_HAL_DIR)/Src/stm32f4xx_ll_gpio.c \
	$(STM32_HAL_DIR)/Src/stm32f4xx_ll_rcc.c \
	$(STM32_HAL_DIR)/Src/stm32f4xx_ll_usart.c \
	$(STM32_HAL_DIR)/Src/stm32f4xx_ll_utils.c

endif