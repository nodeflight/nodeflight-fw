ifeq (1,$(includecount))

include make/nodeflight-stm32-common.mk

FREERTOS_PORT = GCC/ARM_CM7/r0p1

SRC += \
	src/platform/stm32f7xx/platform.c \
	src/platform/stm32f7xx/systime.c \
	src/platform/stm32f7xx/$(STM32_MODEL)/hwmap_$(STM32_MODEL).c

SRC += \
	$(CMSIS_DEVICE_DIR)/Source/Templates/system_stm32f7xx.c \
	$(STM32_HAL_DIR)/Src/stm32f7xx_ll_dma.c \
	$(STM32_HAL_DIR)/Src/stm32f7xx_ll_exti.c \
	$(STM32_HAL_DIR)/Src/stm32f7xx_ll_gpio.c \
	$(STM32_HAL_DIR)/Src/stm32f7xx_ll_rcc.c \
	$(STM32_HAL_DIR)/Src/stm32f7xx_ll_spi.c \
	$(STM32_HAL_DIR)/Src/stm32f7xx_ll_tim.c \
	$(STM32_HAL_DIR)/Src/stm32f7xx_ll_usart.c \
	$(STM32_HAL_DIR)/Src/stm32f7xx_ll_utils.c

endif