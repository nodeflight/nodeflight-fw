include make/verbosity.mk

TARGETS=\
	stm32f722 \
	stm32f745

# For verbosity
TARGET=""

all: $(addprefix build.,$(TARGETS))

build.%: FORCE
	$(TRACE) MAKE $@
	$(Q)$(MAKE) -f make/main.mk TARGET=$*

clean: FORCE
	rm -rf build

FORCE:

.PHONY: FORCE
