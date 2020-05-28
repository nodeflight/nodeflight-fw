#######################################################################
# Application configuration

APP = nodeflight

#######################################################################
# Target configuration

TARGET_stm32f405_DEPENDS = stm32f4xx nodeflight
TARGET_stm32f405_CFLAGS = -DSTM32F405xx
TARGET_stm32f405_LDFLAGS += -Tstm32f405.ld
TARGET_stm32f405_SOURCES = \
	vendor/stm32/cmsis_device_f4/Source/Templates/gcc/startup_stm32f405xx.s \
	platform/stm32f4xx/hwmap_stm32f405.c

TARGET_stm32f722_DEPENDS = stm32f7xx nodeflight
TARGET_stm32f722_CFLAGS = -DSTM32F722xx
TARGET_stm32f722_LDFLAGS += -Tstm32f722.ld
TARGET_stm32f722_SOURCES = \
	vendor/stm32/cmsis_device_f7/Source/Templates/gcc/startup_stm32f722xx.s \
	platform/stm32f7xx/hwmap_stm32f722.c

TARGET_stm32f745_DEPENDS = stm32f7xx nodeflight
TARGET_stm32f745_CFLAGS = -DSTM32F745xx
TARGET_stm32f745_LDFLAGS += -Tstm32f745.ld
TARGET_stm32f745_SOURCES = \
	vendor/stm32/cmsis_device_f7/Source/Templates/gcc/startup_stm32f745xx.s \
	platform/stm32f7xx/hwmap_stm32f745.c

#######################################################################
# Common configuration

TARGET_nodeflight_DEPENDS = freertos tinyprintf core lib
TARGET_nodeflight_SOURCES = \
	core/main.c \
	lib/strops.c \
	module/math/arithmetic.c \
	module/math/calculus.c \
	module/mgmt/nfcp.c \
	module/mgmt/sdcard_spi.c \
	module/mgmt/stdout.c \
	module/rc_io/fport.c \
	module/rc_io/servo.c \
	module/scheduler/sch_decim.c \
	module/scheduler/sch_timer.c

TARGET_nodeflight_INCLUDES = .

TARGET_nodeflight_CFLAGS = \
	-g \
	-O3 \
	-Wdouble-promotion \
	-Wall \
	-Werror \
	-Wpedantic \
	-MMD

TARGET_tinyprintf_SOURCES = \
	vendor/tinyprintf/tinyprintf.c

TARGET_tinyprintf_CFLAGS = \
	-DTINYPRINTF_DEFINE_TFP_PRINTF=1 \
	-DTINYPRINTF_OVERRIDE_LIBC=0 \
	-TINYPRINTF_DEFINE_TFP_SPRINTF=1

TARGET_core_SOURCES = \
	core/config.c \
	core/module.c \
	core/interface.c \
	core/peripheral.c \
	core/resource.c \
	core/scheduler.c \
	core/variable.c

TARGET_lib_SOURCES = \
	lib/strops.c \
	lib/map.c

#######################################################################
# Unit test framework

TARGET_unittest_DEPENDS = lib native
TARGET_unittest_INCLUDES = \
	. \
	unittest
TARGET_unittest_SOURCES = \
	unittest/main.c


#######################################################################
# Platform configuration

TARGET_native_CC      = gcc
TARGET_native_ASM     = gcc
TARGET_native_LD      = gcc
TARGET_native_OBJCOPY = objcopy

TARGET_native_CFLAGS = \
	-g \
	-O3 \
	-Wdouble-promotion \
	-Wall \
	-Werror \
	-Wpedantic \
	-MMD

# Generic Cortex-M

TARGET_arm_CC      = arm-none-eabi-gcc
TARGET_arm_ASM     = arm-none-eabi-gcc
TARGET_arm_LD      = arm-none-eabi-gcc
TARGET_arm_OBJCOPY = arm-none-eabi-objcopy

TARGET_arm_CFLAGS = \
	-Wl,--gc-sections,--sort-section=alignment \
	--specs=nosys.specs \
	--specs=nano.specs \
	--short-enums \
	-fno-builtin \
	-fdata-sections \
	-ffunction-sections \
	-fsingle-precision-constant \
	-fno-strict-aliasing

TARGET_arm_LDFLAGS = \
	$(TARGET_arm_CFLAGS)

# Generic stm32

TARGET_stm32_DEPENDS = arm
TARGET_stm32_INCLUDES = \
	platform/stm32 \
	vendor/stm32/cmsis_core/Core/Include

TARGET_stm32_SOURCES = \
	platform/stm32/peripheral/gpio.c \
	platform/stm32/peripheral/spi.c \
	platform/stm32/peripheral/timer.c \
	platform/stm32/peripheral/uart.c \
	platform/stm32/peripheral/usb_vcp.c \
	platform/stm32/resource/dma.c \
	platform/stm32/resource/gpio.c \
	platform/stm32/hwmap/resource_stm32.c \
	platform/stm32/fault_irq.c

TARGET_stm32_CFLAGS = \
	-DUSE_FULL_LL_DRIVER \
	-DHSE_VALUE=8000000 \
	-mthumb

TARGET_stm32_LDFLAGS = \
	$(TARGET_stm32_CFLAGS) \
	-Wl,-Lsrc/platform/stm32

# Generic stm32f1xx

TARGET_stm32f1xx_DEPENDS = stm32
TARGET_stm32f1xx_INCLUDES = \
	vendor/stm32/cmsis_device_f1/Include \
	vendor/stm32/stm32f1xx_hal_driver/Inc \
	platform/stm32f1xx

TARGET_stm32f1xx_SOURCES = \
	platform/stm32f1xx/platform.c \
	vendor/stm32/cmsis_device_f1/Source/Templates/system_stm32f1xx.c \
	vendor/stm32/stm32f1xx_hal_driver/Src/stm32f1xx_ll_gpio.c \
	vendor/stm32/stm32f1xx_hal_driver/Src/stm32f1xx_ll_rcc.c \
	vendor/stm32/stm32f1xx_hal_driver/Src/stm32f1xx_ll_usart.c

TARGET_stm32f1xx_CFLAGS = \
	-mcpu=cortex-m3 \
	-mfloat-abi=soft

TARGET_stm32f1xx_LDFLAGS = \
	$(TARGET_stm32f1xx_CFLAGS) \
	-Wl,-Lsrc/platform/stm32f1xx

# Generic stm32f4xx

TARGET_stm32f4xx_DEPENDS = stm32
TARGET_stm32f4xx_INCLUDES = \
	vendor/stm32/cmsis_device_f4/Include \
	vendor/stm32/stm32f4xx_hal_driver/Inc \
	platform/stm32f4xx \
	vendor/rtos/FreeRTOS-kernel/portable/GCC/ARM_CM4F

TARGET_stm32f4xx_SOURCES = \
	platform/stm32f4xx/platform.c \
	vendor/stm32/cmsis_device_f4/Source/Templates/system_stm32f4xx.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_dma.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_gpio.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_rcc.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_usart.c \
	vendor/stm32/stm32f4xx_hal_driver/Src/stm32f4xx_ll_utils.c \
	vendor/rtos/FreeRTOS-kernel/portable/GCC/ARM_CM4F/port.c

TARGET_stm32f4xx_CFLAGS = \
	-mcpu=cortex-m4 \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16

TARGET_stm32f4xx_LDFLAGS = \
	$(TARGET_stm32f4xx_CFLAGS) \
	-Wl,-Lsrc/platform/stm32f4xx

# Generic stm32f7xx

TARGET_stm32f7xx_DEPENDS = stm32
TARGET_stm32f7xx_INCLUDES = \
	vendor/stm32/cmsis_device_f7/Include \
	vendor/stm32/stm32f7xx_hal_driver/Inc \
	platform/stm32f7xx \
	vendor/rtos/FreeRTOS-kernel/portable/GCC/ARM_CM7/r0p1

TARGET_stm32f7xx_SOURCES = \
	platform/stm32f7xx/platform.c \
	vendor/stm32/cmsis_device_f7/Source/Templates/system_stm32f7xx.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_dma.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_gpio.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_rcc.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_spi.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_tim.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_usart.c \
	vendor/stm32/stm32f7xx_hal_driver/Src/stm32f7xx_ll_utils.c \
	vendor/rtos/FreeRTOS-kernel/portable/GCC/ARM_CM7/r0p1/port.c

TARGET_stm32f7xx_CFLAGS = \
	-mcpu=cortex-m7 \
	-mfloat-abi=hard \
	-mfpu=fpv5-sp-d16

TARGET_stm32f7xx_LDFLAGS = \
	$(TARGET_stm32f7xx_CFLAGS) \
	-Wl,-Lsrc/platform/stm32f7xx


#######################################################################
# FreeRTOS configuration

TARGET_freertos_INCLUDES = \
	vendor/rtos/FreeRTOS-kernel/include

TARGET_freertos_SOURCES = \
	vendor/rtos/FreeRTOS-kernel/list.c \
	vendor/rtos/FreeRTOS-kernel/queue.c \
	vendor/rtos/FreeRTOS-kernel/tasks.c \
	vendor/rtos/FreeRTOS-kernel/timers.c \
	integration/freertos_hooks.c \
	integration/heap.c 
