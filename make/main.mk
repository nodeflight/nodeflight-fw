include make/config.mk
include make/verbosity.mk

include build/config_$(TARGET).mk

SOURCES=$(CONFIG_SOURCES)
INCLUDES=$(CONFIG_INCLUDES)
CFLAGS=$(CONFIG_CFLAGS)
LDFLAGS=$(CONFIG_LDFLAGS)
CC=$(CONFIG_CC)
ASM=$(CONFIG_ASM)
LD=$(CONFIG_LD)
OBJCOPY=$(CONFIG_OBJCOPY)

CFLAGS+=$(addprefix -Isrc/,$(INCLUDES))

OBJECTS = \
	$(patsubst %.c,build/$(TARGET)/%.o,$(filter %.c,$(SOURCES))) \
	$(patsubst %.s,build/$(TARGET)/%.o,$(filter %.s,$(SOURCES)))


# Common flags regarding build

CFLAGS += -DNF_TARGET=\"$(TARGET)\"

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

build/config_%.mk: make/config.mk
	$(MAKE) -f make/config_deps.mk $@

# Incude dependency tracking
-include $(patsubst %.o,%.d,$(filter %.o,$(OBJECTS)))

.PHONY: all