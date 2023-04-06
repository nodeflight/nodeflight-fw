ifeq (1,$(includecount))

ASM     = $(CC)
LD      = $(CC)
CC      = $(CC)
OBJCOPY = $(OBJCOPY)

CFLAGS += \
	-g \
	-O3 \
	-Wdouble-promotion \
	-Wall \
	-Werror \
	-Wpedantic \
	-MMD \
	-Wno-gnu-zero-variadic-macro-arguments

endif