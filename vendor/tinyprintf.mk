ifeq (1,$(includecount))

TINYPRINTF_DIR:=$(makefile_dir)/tinyprintf

SRC += \
	$(TINYPRINTF_DIR)/tinyprintf.c

INC += \
	$(TINYPRINTF_DIR)

CFLAGS += \
	-DTINYPRINTF_DEFINE_TFP_PRINTF=0 \
	-DTINYPRINTF_DEFINE_TFP_SPRINTF=1 \
	-DTINYPRINTF_OVERRIDE_LIBC=0

endif
