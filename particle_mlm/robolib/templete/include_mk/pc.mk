# libkairo



TOOLS_PREFIX = /usr/bin/

CC = $(TOOLS_PREFIX)gcc

AR = $(TOOLS_PREFIX)ar





# robolib
ROBOLIB = $(ROBOLIB_PATH)/robolib.a


# compile option

INCLUDE_OPTS = $(shell find $(ROBOLIB_PATH) -mindepth 2 -name Makefile | sed -e 's/\(^.*\)\/Makefile$$/-I\1/' | sed -e '/doc/d' | sed -e '/templete/d') -I$(ROBOLIB_PATH)/heads/hal
CFLAGS = -MD -W -Wall $(OPTIMIZATION_OPT) $(INCLUDE_OPTS)

ARFLAGS = rc
