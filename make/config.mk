#######################################################################
# Application configuration

APP = nodeflight

#######################################################################
# Target configuration

TARGET_stm32f722_PLATFORM = stm32f7xx
TARGET_stm32f722_CFLAGS = -DSTM32F722xx
TARGET_stm32f722_LDFLAGS += -Tstm32f722.ld
TARGET_stm32f722_SOURCES = \
	platform/stm32f7xx/cmsis_device_f7/Source/Templates/gcc/startup_stm32f722xx.s

TARGET_stm32f745_PLATFORM = stm32f7xx
TARGET_stm32f745_CFLAGS = -DSTM32f745xx
TARGET_stm32f745_LDFLAGS += -Tstm32f745.ld
TARGET_stm32f745_SOURCES = \
	platform/stm32f7xx/cmsis_device_f7/Source/Templates/gcc/startup_stm32f745xx.s


#######################################################################
# Platform configuration

PLATFORM_stm32f7xx_INCLUDES = \
	platform/stm32/cmsis_core/Core/Include \
	platform/stm32f7xx/cmsis_device_f7/Include \
	platform/stm32f7xx/stm32f7xx_hal_driver/Inc \
	platform/stm32f7xx

PLATFORM_stm32f7xx_SOURCES = \
	platform/stm32f7xx/cmsis_device_f7/Source/Templates/system_stm32f7xx.c \
	platform/stm32f7xx/stm32f7xx_hal_driver/Src/stm32f7xx_hal_cortex.c \
	platform/stm32f7xx/stm32f7xx_hal_driver/Src/stm32f7xx_hal_dma.c \
	platform/stm32f7xx/stm32f7xx_hal_driver/Src/stm32f7xx_hal_gpio.c \
	platform/stm32f7xx/stm32f7xx_hal_driver/Src/stm32f7xx_hal_rcc.c \
	platform/stm32f7xx/stm32f7xx_hal_driver/Src/stm32f7xx_hal_uart.c \
	platform/stm32f7xx/stm32f7xx_hal_driver/Src/stm32f7xx_hal.c

PLATFORM_stm32f7xx_CFLAGS = \
	-mthumb \
	-mcpu=cortex-m7 \
	-mfloat-abi=hard \
	-mfpu=fpv5-sp-d16 \
	-fsingle-precision-constant \
	-Wdouble-promotion \
	-MMD

PLATFORM_stm32f7xx_LDFLAGS = \
	$(PLATFORM_stm32f7xx_CFLAGS) \
	-Wl,-Lsrc/platform/stm32f7xx

PLATFORM_stm32f7xx_CC      = arm-none-eabi-gcc
PLATFORM_stm32f7xx_ASM     = arm-none-eabi-gcc
PLATFORM_stm32f7xx_LD      = arm-none-eabi-gcc
PLATFORM_stm32f7xx_OBJCOPY = arm-none-eabi-objcopy

#######################################################################
# Common configuration

COMMON_SOURCES = \
	core/main.c \
	core/system.c

COMMON_CFLAGS = \
	-g