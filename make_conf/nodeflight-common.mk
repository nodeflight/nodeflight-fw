ifeq (1,$(includecount))

# nodeflight

DEPENDS += nfcp

SOURCEDIRS += \
	core \
	lib \
	integration

INCLUDES += .

# freertos

INCLUDES += \
	vendor/rtos/FreeRTOS-kernel/include

SOURCES += \
	vendor/rtos/FreeRTOS-kernel/list.c \
	vendor/rtos/FreeRTOS-kernel/queue.c \
	vendor/rtos/FreeRTOS-kernel/tasks.c \
	vendor/rtos/FreeRTOS-kernel/timers.c

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

SOURCEDIRS += \
	integration/fatfs

SOURCES += \
	vendor/fatfs/ff.c \
	vendor/fatfs/ffunicode.c

endif
