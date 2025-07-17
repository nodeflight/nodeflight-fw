ifeq (1,$(includecount))

FREERTOS_DIR:=$(makefile_dir)/freertos

SRC += \
	$(FREERTOS_DIR)/croutine.c \
	$(FREERTOS_DIR)/event_groups.c \
	$(FREERTOS_DIR)/list.c \
	$(FREERTOS_DIR)/queue.c \
	$(FREERTOS_DIR)/stream_buffer.c \
	$(FREERTOS_DIR)/tasks.c \
	$(FREERTOS_DIR)/timers.c \
	$(FREERTOS_DIR)/portable/$(FREERTOS_PORT)/port.c

INC += \
	$(FREERTOS_DIR)/include \
	$(FREERTOS_DIR)/portable/$(FREERTOS_PORT)

endif
