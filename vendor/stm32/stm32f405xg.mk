ifeq (1,$(includecount))

# Will set CBE_ARCH_STM32_DIR
include $(makefile_dir)/stm32f4xx.mk

STM32_MODEL=stm32f405

CFLAGS += \
	-D$(STM32_MODEL)xx
	
LINKFLAGS += \
	-Wl,-Lsrc/linker \
	-TSTM32F405xG.ld

endif
