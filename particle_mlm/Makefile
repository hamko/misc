PKG = product
SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)

LIBHAL_PATH = ./sim_library/hal_devices
LIBHAL = $(LIBHAL_PATH)/libhal.a

LIBHDW_PATH = ./sim_library/hardware_dependent
LIBHDW = $(LIBHDW_PATH)/libhdw.a

LIBWLD_PATH = ./sim_library/world
LIBWLD = $(LIBWLD_PATH)/libwld.a

LIBGUI_PATH = ./sim_library/gui
LIBGUI = $(LIBGUI_PATH)/libgui.a

ROBOLIB_PATH = ./robolib
ROBOLIB = $(ROBOLIB_PATH)/robolib.a
ROBOLIB_IPATH = $(shell find $(ROBOLIB_PATH) -mindepth 2 -name Makefile | sed -e 's/\(^.*\)\/Makefile$$/-I\1/' | sed -e '/doc/d')

DM = ./sim_library/device_manager.o
HAL = ./sim_library/hal.o

IPATH = -I./sim_library -I$(LIBHAL_PATH) $(ROBOLIB_IPATH) -I./sim_library/hardware_dependent/init -I./sim_library/world

DEBUG = _N_DEBUG_

.PHONY: all full lib clean full_clean
all: $(PKG)

full: lib $(PKG)

lib:
	make -C sim_library
	make -C robolib

$(PKG): $(HAL) $(DM) $(ROBOLIB) $(OBJS) $(LIBHAL) $(LIBHDW) $(LIBGUI)
	gcc -O2 -W -Wall -o $@ \
	$(OBJS) $(HAL) $(ROBOLIB) $(DM) $(LIBHDW) $(LIBHAL) $(LIBWLD) $(LIBGUI) \
	-lpthread -lm `pkg-config --cflags --libs gtk+-2.0`

.c.o:
	gcc -O2 -W -Wall $(IPATH) -D$(DEBUG) -o $@ -c -MD $<
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d

$(ROBOLIB):
	make -C robolib

$(HAL):
	make -C sim_library

$(DM):
	make -C sim_library

clean:
	rm -f $(PKG)* $(OBJS) *~ *.P

full_clean:
	make clean
	make -C sim_library clean
	make -C robolib clean

-include $(SRCS:.c=.P)

