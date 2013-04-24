# libkairo



TOOLS_PREFIX = /usr/local/h8300-elf/bin/h8300-elf-

CC = $(TOOLS_PREFIX)gcc

AR = $(TOOLS_PREFIX)ar





# robolib
ROBOLIB = $(ROBOLIB_PATH)/robolib.a


# compile option

INCLUDE_OPTS = $(shell find $(ROBOLIB_PATH) -mindepth 2 -name Makefile | sed -e 's/\(^.*\)\/Makefile$$/-I\1/' | sed -e '/doc/d' | sed -e '/templete/d') -I$(ROBOLIB_PATH)/../2008_board_library -I$(ROBOLIB_PATH)/../2008_board_library/hal_devices
CFLAGS = -MD -W -Wall -mh $(OPTIMIZATION_OPT) $(INCLUDE_OPTS)

ARFLAGS = rc
