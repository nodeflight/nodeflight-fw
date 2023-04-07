include make/tools.mk

TARGETS=\
	nodeflight-stm32f722xe \
	nodeflight-stm32f745xg

# Disabled: nodeflight-stm32f405

# For verbosity
TARGET=""

all: $(addsuffix .elf,$(TARGETS))

check: unittest

%.elf %.hex: FORCE
	$(TRACE) MAKE $@
	$(Q)$(MAKE) $(SILENT) -f make/target.mk TARGET=$*

unittest: build/nodeflight-unittest.elf
	$(TRACE) UNITTEST
	$(Q)$<

build/%.elf: FORCE
	$(TRACE) MAKE $@
	$(Q)$(MAKE) $(SILENT) -q -f make/target.mk TARGET=$* build/nodeflight-$*.elf

clean: FORCE
	rm -rf build
	rm -f $(addsuffix .elf,$(TARGETS))
	rm -f $(addsuffix .hex,$(TARGETS))

FORCE:

.PHONY: FORCE
