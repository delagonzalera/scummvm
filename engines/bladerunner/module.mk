MODULE := engines/bladerunner

MODULE_OBJS = \
	archive.o \
	aud_decoder.o \
	bladerunner.o \
	chapters.o \
	decompress_lcw.o \
	decompress_lzo.o \
	detection.o \
	gameinfo.o \
	image.o \
	outtake.o \
	settings.o \
	vqa_decoder.o

# This module can be built as a plugin
ifeq ($(ENABLE_BLADERUNNER), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
