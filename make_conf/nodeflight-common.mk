ifeq (1,$(includecount))

# nodeflight

DEPENDS += nfcp

SOURCES += \
	core/config.c \
	core/disk_access.c \
	core/disk_int.c \
	core/module.c \
	core/interface.c \
	core/log.c \
	core/peripheral.c \
	core/resource.c \
	core/scheduler.c \
	core/variable.c \
	core/random.c \
	core/main.c \
	lib/crc.c \
	lib/hdlc.c \
	lib/strops.c \
	lib/map.c \

INCLUDES += .

# freertos

INCLUDES += \
	vendor/rtos/FreeRTOS-kernel/include

SOURCES += \
	vendor/rtos/FreeRTOS-kernel/list.c \
	vendor/rtos/FreeRTOS-kernel/queue.c \
	vendor/rtos/FreeRTOS-kernel/tasks.c \
	vendor/rtos/FreeRTOS-kernel/timers.c \
	integration/freertos_hooks.c \
	integration/heap.c 

# tinyprintf

SOURCES += \
	vendor/tinyprintf/tinyprintf.c

CFLAGS += \
	-DTINYPRINTF_DEFINE_TFP_PRINTF=0 \
	-DTINYPRINTF_OVERRIDE_LIBC=0 \
	-TINYPRINTF_DEFINE_TFP_SPRINTF=1
	
# fatfs

INCLUDES += \
	integration/fatfs \
	vendor/fatfs

SOURCES += \
	integration/fatfs/ffsystem.c \
	vendor/fatfs/ff.c \
	vendor/fatfs/ffunicode.c

endif
