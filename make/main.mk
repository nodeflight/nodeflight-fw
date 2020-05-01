include make/config.mk
include make/verbosity.mk

PLATFORM=$(TARGET_$(TARGET)_PLATFORM)

ifeq ($(PLATFORM),)
$(error Unknown target $(TARGET))
endif

AGGREGATED_VARS=\
	SOURCES \
	INCLUDES \
	CFLAGS \
	LDFLAGS \
	CC \
	ASM \
	LD \
	OBJCOPY

define fillvar
$(var) = \
	$$(TARGET_$(TARGET)_$(var)) \
	$$(PLATFORM_$(PLATFORM)_$(var)) \
	$$(COMMON_$(var))
endef
$(foreach var,$(AGGREGATED_VARS),$(eval $(fillvar)))

CFLAGS+=$(addprefix -Isrc/,$(INCLUDES))

OBJECTS = \
	$(patsubst %.c,build/$(TARGET)/%.o,$(filter %.c,$(SOURCES))) \
	$(patsubst %.s,build/$(TARGET)/%.o,$(filter %.s,$(SOURCES)))

all: build/$(APP)-$(TARGET).elf build/$(APP)-$(TARGET).hex

build/$(APP)-$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(@D)
	$(TRACE) LD $@
	$(Q)$(LD) $(LDFLAGS) -o $@ $^

%.hex: %.elf
	$(TRACE) OBJCOPY $@
	$(Q)$(OBJCOPY) -O ihex $< $@

build/$(TARGET)/%.o: src/%.c
	@mkdir -p $(@D)
	$(TRACE) CC $<
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

build/$(TARGET)/%.o: src/%.s
	@mkdir -p $(@D)
	$(TRACE) ASM $<
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

# Incude dependency tracking
-include $(OBJECTS:.o=.d)

.PHONY: all