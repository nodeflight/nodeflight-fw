ifeq ($(V),1)
TRACE = @printf -- "----- %-10s %-8s %s\n" $(TARGET)
Q     =
else
TRACE = @printf "%-10s %-8s %s\n" $(TARGET)
Q     = @
endif