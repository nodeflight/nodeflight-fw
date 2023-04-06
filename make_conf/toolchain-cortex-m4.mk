ifeq (1,$(includecount))

include make_conf/toolchain-armv7m.mk

cortex_m4_FLAGS = \
	-mcpu=cortex-m4 \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16

CFLAGS += $(cortex_m4_FLAGS)
LDFLAGS += $(cortex_m4_FLAGS)

endif