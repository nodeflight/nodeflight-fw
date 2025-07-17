ifeq (1,$(includecount))

# Will set CBE_ARCH_STM32_DIR
include $(makefile_dir)/stm32f7xx.mk

STM32_MODEL=stm32f722

CFLAGS += \
	-DSTM32F722xx
	
LINKFLAGS += \
	-Wl,-Lsrc/linker \
	-TSTM32F722xE.ld

endif
