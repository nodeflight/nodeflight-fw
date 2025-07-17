ifeq (1,$(includecount))

CBE_ARCH_STM32_DIR := $(makefile_dir)

STM32_FAMILY=stm32$(CORTEX_DEVICE)xx

################################################################################
# Paths
################################################################################

CMSIS_CORE_DIR   := $(CBE_ARCH_STM32_DIR)/cmsis_core
CMSIS_DEVICE_DIR := $(CBE_ARCH_STM32_DIR)/cmsis_device_$(CORTEX_DEVICE)
STM32_HAL_DIR    := $(CBE_ARCH_STM32_DIR)/$(STM32_FAMILY)_hal_driver

INC+=\
	$(CBE_ARCH_STM32_DIR)/cmsis_core/Include \
	$(CMSIS_DEVICE_DIR)/Include \
	$(STM32_HAL_DIR)/Inc

SRC += \
	$(CMSIS_DEVICE_DIR)/Source/Templates/gcc/startup_$(STM32_MODEL)xx.s \
	$(CMSIS_DEVICE_DIR)/Source/Templates/system_$(STM32_FAMILY).c

################################################################################
# Toolchain
################################################################################

CC      = arm-none-eabi-gcc
ASM     = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# --specs=nosys.specs \

# ARMv7-M common flags
CFLAGS += \
	-Wl,--gc-sections,--sort-section=alignment \
	--specs=nano.specs \
	--short-enums \
	-fno-builtin \
	-fdata-sections \
	-ffunction-sections \
	-fsingle-precision-constant \
	-fno-strict-aliasing \
	-mthumb

endif
