#######################################################################
# Application configuration

APP = nodeflight

#######################################################################
# Target configuration

TARGET_stm32f405_PLATFORM = arm stm32 stm32f4xx
TARGET_stm32f405_CFLAGS = -DSTM32F405xx
TARGET_stm32f405_LDFLAGS += -Tstm32f405.ld
TARGET_stm32f405_SOURCES = \
	vendor/stm32/cmsis_device_f4/Source/Templates/gcc/startup_stm32f405xx.s \
	platform/stm32f4xx/resource_instances_stm32f405.c

TARGET_stm32f722_PLATFORM = arm stm32 stm32f7xx
TARGET_stm32f722_CFLAGS = -DSTM32F722xx
TARGET_stm32f722_LDFLAGS += -Tstm32f722.ld
TARGET_stm32f722_SOURCES = \
	vendor/stm32/cmsis_device_f7/Source/Templates/gcc/startup_stm32f722xx.s \
	platform/stm32f7xx/resource_instances_stm32f722.c

TARGET_stm32f745_PLATFORM = arm stm32 stm32f7xx
TARGET_stm32f745_CFLAGS = -DSTM32F745xx
TARGET_stm32f745_LDFLAGS += -Tstm32f745.ld
TARGET_stm32f745_SOURCES = \
	vendor/stm32/cmsis_device_f7/Source/Templates/gcc/startup_stm32f745xx.s \
	platform/stm32f7xx/resource_instances_stm32f745.c


#######################################################################
# Platform configuration

# Generic Cortex-M

PLATFORM_arm_CC      = arm-none-eabi-gcc
PLATFORM_arm_ASM     = arm-none-eabi-gcc
PLATFORM_arm_LD      = arm-none-eabi-gcc
PLATFORM_arm_OBJCOPY = arm-none-eabi-objcopy

PLATFORM_arm_CFLAGS = \
	-Wl,--gc-sections,--sort-section=alignment \
	--specs=nosys.specs \
	--specs=nano.specs \
	--short-enums \
	-fno-builtin \
	-fdata-sections \
	-ffunction-sections \
	-fsingle-precision-constant \
	-fno-strict-aliasing

PLATFORM_arm_LDFLAGS = \
	$(PLATFORM_arm_CFLAGS)

# Generic stm32

PLATFORM_stm32_INCLUDES = \
	vendor/stm32/cmsis_core/Core/Include

PLATFORM_stm32_SOURCES = \
	platform/stm32/resource/uart.c \
	platform/stm32/resource/usb_vcp.c

PLATFORM_stm32_CFLAGS = \
	-DUSE_FULL_LL_DRIVER \
	-mthumb

PLATFORM_stm32_LDFLAGS = \
	$(PLATFORM_stm32_CFLAGS) \
	-Wl,-Lsrc/platform/stm32

# Generic stm32f1xx

PLATFORM_stm32f1xx_INCLUDES = \
	vendor/stm32/cmsis_device_f1/Include \
	vendor/stm32/stm32f1xx_hal_driver/Inc \
	platform/stm32f1xx

PLATFORM_stm32f1xx_SOURCES = \
	platform/stm32f1xx/platform.c \
	vendor/stm32/cmsis_device_f1/Source/Templates/system_stm32f1xx.c \
	vendor/stm32/stm32f1xx_hal_driver/Src/stm32f1xx_ll_gpio.c \
	vendor/stm32/stm32f1xx_hal_driver/Src/stm32f1xx_ll_rcc.c \
	vendor/stm32/stm32f1xx_hal_driver/Src/stm32f1xx_ll_usart.c

PLATFORM_stm32f1xx_CFLAGS = \
	-mcpu=cortex-m3 \
	-mfloat-abi=soft

PLATFORM_stm32f1xx_LDFLAGS = \
	$(PLATFORM_stm32f1xx_CFLAGS) \
	-Wl,-Lsrc/platform/stm32f1xx

# Generic stm32f4xx

PLATFORM_stm32f4xx_INCLUDES = \
	vendor/stm32/cmsis_device_f4/Include \
	vendor/stm32/stm32f4xx_hal_driver/Inc \
	platform/stm32f4xx

PLATFORM_stm32f4xx_SOURCES = \
	platform/stm32f4xx/platform.c \
	vendor/stm32/cmsis_device_f4/Source/Templates/system_stm32f4xx.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_gpio.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_rcc.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_usart.c

PLATFORM_stm32f4xx_CFLAGS = \
	-mcpu=cortex-m4 \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16

PLATFORM_stm32f4xx_LDFLAGS = \
	$(PLATFORM_stm32f4xx_CFLAGS) \
	-Wl,-Lsrc/platform/stm32f4xx

# Generic stm32f7xx

PLATFORM_stm32f7xx_INCLUDES = \
	vendor/stm32/cmsis_device_f7/Include \
	vendor/stm32/stm32f7xx_hal_driver/Inc \
	platform/stm32f7xx

PLATFORM_stm32f7xx_SOURCES = \
	platform/stm32f7xx/platform.c \
	vendor/stm32/cmsis_device_f7/Source/Templates/system_stm32f7xx.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_gpio.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_rcc.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_usart.c

PLATFORM_stm32f7xx_CFLAGS = \
	-mcpu=cortex-m7 \
	-mfloat-abi=hard \
	-mfpu=fpv5-sp-d16

PLATFORM_stm32f7xx_LDFLAGS = \
	$(PLATFORM_stm32f7xx_CFLAGS) \
	-Wl,-Lsrc/platform/stm32f7xx

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