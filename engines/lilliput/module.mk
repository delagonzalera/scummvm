MODULE := engines/lilliput

MODULE_OBJS = \
	console.o \
	detection.o \
	lilliput.o \
	script.o

MODULE_DIRS += \
	engines/lilliput

# This module can be built as a plugin
ifeq ($(ENABLE_LILLIPUT), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
