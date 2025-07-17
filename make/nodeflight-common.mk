ifeq (1,$(includecount))

CFLAGS += \
	-DNF_TARGET=\"\\\"$(patsubst %.elf,%,$(TARGET))\\\"\"

CFLAGS += \
	-g \
	-O3 \
	-Wdouble-promotion \
	-Wall \
	-Werror \
	-Wpedantic \
	-MMD

SRC += \
	src/core/config.c                                  \
	src/core/disk_access.c                             \
	src/core/disk_int.c                                \
	src/core/interface.c                               \
	src/core/log.c                                     \
	src/core/main.c                                    \
	src/core/module.c                                  \
	src/core/peripheral.c                              \
	src/core/random.c                                  \
	src/core/resource.c                                \
	src/core/scheduler.c                               \
	src/core/task_trace.c                              \
	src/core/variable.c                                \
	src/integration/fatfs/ffsystem.c                   \
	src/integration/freertos_hooks.c                   \
	src/integration/heap.c                             \
	src/lib/crc.c                                      \
	src/lib/hdlc.c                                     \
	src/lib/map.c                                      \
	src/lib/strops.c                                   \
	src/module/math/arithmetic.c                       \
	src/module/math/calculus.c                         \
	src/module/mgmt/sdcard.c                           \
	src/module/mgmt/stdout.c                           \
	src/module/nfcp/nfcp.c                             \
	src/module/nfcp/nfcp_cls_cap.c                     \
	src/module/nfcp/nfcp_cls_mgmt.c                    \
	src/module/rc_io/fport.c                           \
	src/module/rc_io/servo.c                           \
	src/module/scheduler/sch_decim.c                   \
	src/module/scheduler/sch_timer.c                   \
	src/module/sensor/motion_mpu6500.c

UNITTEST_SRC += \
	src/lib/test/crc.test.c                            \
	src/unittest/main.c

endif