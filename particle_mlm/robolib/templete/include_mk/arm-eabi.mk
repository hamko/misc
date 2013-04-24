# libkairo
LIBKAIRO_INSTALL_DIR = /usr/local/share/libkairo
LIBKAIRO_HEADS = /usr/local/arm-eabi/arm-eabi/include/include

TOOLS_PREFIX = /usr/local/arm-eabi/bin/arm-eabi-

CC = $(TOOLS_PREFIX)gcc
AS = $(TOOLS_PREFIX)as
AR = $(TOOLS_PREFIX)ar
LD = $(TOOLS_PREFIX)gcc
OBJCOPY = $(TOOLS_PREFIX)objcopy

LINKER_SCRIPT = $(LIBKAIRO_INSTALL_DIR)/stm32.ld

# robolib
ROBOLIB = $(ROBOLIB_PATH)/robolib.a
ROBOLIB_HEADS = $(ROBOLIB_PATH)/heads

# compile option
OPTIMIZATION_OPTS = -O2
INCLUDE_OPTS = -I$(ROBOLIB_HEADS) -I$(LIBKAIRO_HEADS) $(shell find $(ROBOLIB_PATH) -mindepth 2 -name Makefile | sed -e 's/\(^.*\)\/Makefile$$/-I\1/' | sed -e '/doc/d' | sed -e '/templete/d')
CFLAGS = -mcpu=cortex-m3 -mthumb -W -Wall $(OPTIMIZATION_OPT) $(INCLUDE_OPTS)
LDFLAGS = -Wl,--gc-sections,-Map=$@.map,-cref,-u,reset_handler -T $(LINKER_SCRIPT)
ARFLAGS = rc
