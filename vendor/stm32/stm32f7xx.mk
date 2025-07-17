ifeq (1,$(includecount))

CORTEX_DEVICE=f7
STM32_FAMILY=stm32f7xx

# Will set CBE_ARCH_STM32_DIR
include $(makefile_dir)/stm32.mk

################################################################################
# Toolchain
################################################################################

# Cortex-M7 flags
CFLAGS += \
	-mcpu=cortex-m7 \
	-mfloat-abi=hard \
	-mfpu=fpv5-sp-d16

################################################################################
# Integration
################################################################################

FREERTOS_PORT=GCC/ARM_CM7/r0p1

endif
