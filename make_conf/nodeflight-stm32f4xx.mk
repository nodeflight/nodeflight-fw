ifeq (1,$(includecount))

include make_conf/toolchain-cortex-m4.mk
include make_conf/nodeflight-stm32.mk

INCLUDES += \
	vendor/stm32/cmsis_device_f4/Include \
	vendor/stm32/stm32f4xx_hal_driver/Inc \
	platform/stm32f4xx \
	vendor/rtos/FreeRTOS-kernel/portable/GCC/ARM_CM4F

SOURCES += \
	platform/stm32f4xx/platform.c \
	vendor/rtos/FreeRTOS-kernel/portable/GCC/ARM_CM4F/port.c \
	vendor/stm32/cmsis_device_f4/Source/Templates/system_stm32f4xx.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_dma.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_gpio.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_rcc.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_usart.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_utils.c

endif