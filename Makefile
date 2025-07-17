TARGETS=$(patsubst targets/%.mk,%,$(wildcard targets/*.mk))

include vendor/cbe/load.mk
