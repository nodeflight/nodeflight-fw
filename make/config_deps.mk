include make/config.mk
include make/verbosity.mk

PREFIX?=

AGGREGATED_VARS=\
	SOURCES \
	INCLUDES \
	CFLAGS \
	LDFLAGS \
	CC \
	ASM \
	LD \
	OBJCOPY

build/config$(PREFIX)_%.mk: make/config.mk
	@mkdir -p $(@D)
	@if [ "$(TARGET_$*_DEPENDS)" != "" ]; then \
		$(MAKE) -f make/config_deps.mk PREFIX="$(PREFIX)_$*" $(patsubst %,build/config$(PREFIX)_$*_%.mk, $(TARGET_$*_DEPENDS)); \
	fi
	$(TRACE) "GEN_MK" $@
	@true > $@
	@if [ "$(TARGET_$*_DEPENDS)" != "" ]; then \
		cat $(patsubst %,build/config$(PREFIX)_$*_%.mk, $(TARGET_$*_DEPENDS)) >> $@; \
	fi
	@echo "" >> $@
	@echo "# target=$*" >> $@
	$(Q){ $(foreach var,$(AGGREGATED_VARS),echo "CONFIG_$(var)+=$(TARGET_$*_$(var))";) } >> $@