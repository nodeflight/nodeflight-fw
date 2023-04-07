# Reduce verbosity for targets
#
# Usage: Add $Q before each line in a step, and add $(TRACE) with a message
# before.
#
# do_stuff:
#     $(TRACE) STUFF $@
#     $(Q)command stuff to do
#     $(Q)other_command more stuff to do
#
ifeq ($(V),1)
TRACE  = @printf -- "----- %-24s %-8s %s\n" $(TARGET)
Q      =
SILENT =
else
TRACE  = @printf "%-24s %-8s %s\n" $(TARGET)
Q      = @
SILENT = --silent
endif

# Guard against including same makefile several times
#
# ifeq (1,$(includecount))
# ...
# endif
includecount=$(words $(filter $(lastword $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# Wildcard function, but originating around the src/ directory.
#
# To allow auto-configuration within make_conf, related to vendor directories
srcwc=$(patsubst src/%,%,$(wildcard $(addprefix src/,$1)))
