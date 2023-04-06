ifeq (1,$(includecount))

include make_conf/toolchain-cortex-m7.mk
include make_conf/nodeflight-stm32.mk

INCLUDES += \
	vendor/stm32/cmsis_device_f7/Include \
	vendor/stm32/stm32f7xx_hal_driver/Inc \
	platform/stm32f7xx \
	vendor/rtos/FreeRTOS-kernel/portable/GCC/ARM_CM7/r0p1

SOURCES += \
	platform/stm32f7xx/platform.c \
	vendor/rtos/FreeRTOS-kernel/portable/GCC/ARM_CM7/r0p1/port.c \
	vendor/stm32/cmsis_device_f7/Source/Templates/system_stm32f7xx.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_dma.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_exti.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_gpio.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_rcc.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_spi.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_tim.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_usart.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_utils.c

endif