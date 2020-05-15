include make/verbosity.mk

TARGETS=\
	stm32f722 \
	stm32f745

# For verbosity
TARGET=""

all: $(addprefix build.,$(TARGETS))

check: unittest

build.%: FORCE
	$(TRACE) MAKE $@
	$(Q)$(MAKE) -f make/main.mk TARGET=$*

unittest: build/nodeflight-unittest.elf
	$(TRACE) UNITTEST
	$(Q)$<

build/nodeflight-%.elf: FORCE
	$(TRACE) MAKE $@
	$(Q)$(MAKE) -f make/main.mk TARGET=$* build/nodeflight-$*.elf

clean: FORCE
	rm -rf build

FORCE:

.PHONY: FORCE
