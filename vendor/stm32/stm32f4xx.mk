ifeq (1,$(includecount))

CORTEX_DEVICE=f4
STM32_FAMILY=stm32f4xx

# Will set CBE_ARCH_STM32_DIR
include $(makefile_dir)/stm32.mk

################################################################################
# Toolchain
################################################################################

# Cortex-M4 flags
CFLAGS += \
	-mcpu=cortex-m4 \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16

################################################################################
# Integration
################################################################################

FREERTOS_PORT=GCC/ARM_CM4F

endif
