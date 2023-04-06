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
TRACE = @printf -- "----- %-10s %-8s %s\n" $(TARGET)
Q     =
else
TRACE = @printf "%-10s %-8s %s\n" $(TARGET)
Q     = @
endif

# Guard against including same makefile several times
#
# ifeq (1,$(includecount))
# ...
# endif
includecount=$(words $(filter $(lastword $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))