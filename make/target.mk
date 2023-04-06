include make/tools.mk
include make_conf/target-$(TARGET).mk

CFLAGS+=$(addprefix -Isrc/,$(INCLUDES))
SOURCES += $(patsubst src/%,%,$(foreach dir,$(SOURCEDIRS),$(wildcard src/$(dir)/*.c src/(dir)/*.s)))

OBJECTS = \
	$(patsubst %.c,build/$(TARGET)/%.o,$(filter %.c,$(SOURCES))) \
	$(patsubst %.s,build/$(TARGET)/%.o,$(filter %.s,$(SOURCES)))

# Common flags regarding build

CFLAGS += -DNF_TARGET=\"$(TARGET)\"

all: $(TARGET).elf $(TARGET).hex

$(TARGET).elf: $(OBJECTS)
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
-include $(patsubst %.o,%.d,$(filter %.o,$(OBJECTS)))

.PHONY: all