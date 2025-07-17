ifeq (1,$(includecount))

# Will set CBE_ARCH_STM32_DIR
include $(makefile_dir)/stm32f7xx.mk

STM32_MODEL=stm32f746

CFLAGS += \
	-DSTM32F746xx
	
LINKFLAGS += \
	-Wl,-Lsrc/linker \
	-TSTM32F746xG.ld

endif
