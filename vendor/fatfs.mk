ifeq (1,$(includecount))

FATFS_DIR:=$(makefile_dir)/fatfs

SRC += \
	$(FATFS_DIR)/ff.c \
	$(FATFS_DIR)/ffunicode.c

INC += \
	$(FATFS_DIR)

endif
