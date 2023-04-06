ifeq (1,$(includecount))

CC      = arm-none-eabi-gcc
ASM     = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS += \
	-g \
	-O3 \
	-Wdouble-promotion \
	-Wall \
	-Werror \
	-Wpedantic \
	-MMD

armv7_CFLAGS = \
	-Wl,--gc-sections,--sort-section=alignment \
	--specs=nosys.specs \
	--specs=nano.specs \
	--short-enums \
	-fno-builtin \
	-fdata-sections \
	-ffunction-sections \
	-fsingle-precision-constant \
	-fno-strict-aliasing \
	-mthumb

CFLAGS += $(armv7_CFLAGS)
LDFLAGS += $(armv7_CFLAGS)

endif