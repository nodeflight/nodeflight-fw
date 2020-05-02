#######################################################################
# Application configuration

APP = nodeflight

#######################################################################
# Target configuration

TARGET_stm32f722_PLATFORM = stm32f7xx
TARGET_stm32f722_CFLAGS = -DSTM32F722xx
TARGET_stm32f722_LDFLAGS += -Tstm32f722.ld
TARGET_stm32f722_SOURCES = \
	vendor/stm32/cmsis_device_f7/Source/Templates/gcc/startup_stm32f722xx.s

TARGET_stm32f745_PLATFORM = stm32f7xx
TARGET_stm32f745_CFLAGS = -DSTM32F745xx
TARGET_stm32f745_LDFLAGS += -Tstm32f745.ld
TARGET_stm32f745_SOURCES = \
	vendor/stm32/cmsis_device_f7/Source/Templates/gcc/startup_stm32f745xx.s


#######################################################################
# Platform configuration

PLATFORM_stm32f7xx_INCLUDES = \
	vendor/stm32/cmsis_core/Core/Include \
	vendor/stm32/cmsis_device_f7/Include \
	vendor/stm32/stm32f7xx_hal_driver/Inc \
	platform/stm32f7xx

PLATFORM_stm32f7xx_SOURCES = \
	vendor/stm32/cmsis_device_f7/Source/Templates/system_stm32f7xx.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_hal_cortex.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_hal_dma.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_hal_gpio.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_hal_rcc.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_hal_uart.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_hal.c \
	platform/stm32f7xx/platform.c \
	platform/stm32f7xx/resource/uart.c \
	platform/stm32f7xx/resource/usb_vcp.c

PLATFORM_stm32f7xx_CFLAGS = \
	--short-enums \
	-fno-builtin \
	-fdata-sections \
	-ffunction-sections \
	-fsingle-precision-constant \
	-fno-strict-aliasing \
	-mcpu=cortex-m7 \
	-mfloat-abi=hard \
	-mfpu=fpv5-sp-d16 \
	-mthumb

PLATFORM_stm32f7xx_LDFLAGS = \
	$(PLATFORM_stm32f7xx_CFLAGS) \
	-Wl,-Lsrc/platform/stm32f7xx \
	-Wl,--gc-sections,--sort-section=alignment \
	--specs=nosys.specs \
	--specs=nano.specs

PLATFORM_stm32f7xx_CC      = arm-none-eabi-gcc
PLATFORM_stm32f7xx_ASM     = arm-none-eabi-gcc
PLATFORM_stm32f7xx_LD      = arm-none-eabi-gcc
PLATFORM_stm32f7xx_OBJCOPY = arm-none-eabi-objcopy

#######################################################################
# Common configuration

COMMON_SOURCES = \
	core/main.c \
	core/stdout.c \
	resource/resource.c

COMMON_INCLUDES = .

COMMON_CFLAGS = \
	-g \
	-O3 \
	-Wdouble-promotion \
	-Wall \
	-Werror \
	-Wpedantic \
	-MMD