ifeq (1,$(includecount))

include make_conf/module-math.mk
include make_conf/module-mgmt.mk
include make_conf/module-nfcp.mk
include make_conf/module-rc_io.mk
include make_conf/module-scheduler.mk
include make_conf/module-sensor.mk

endif