ifeq (1,$(includecount))

include make_conf/toolchain-armv7m.mk

cortex_m7_FLAGS = \
	-mcpu=cortex-m7 \
	-mfloat-abi=hard \
	-mfpu=fpv5-sp-d16

CFLAGS += $(cortex_m7_FLAGS)
LDFLAGS += $(cortex_m7_FLAGS)

endif