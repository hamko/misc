SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)

HAL_PATH = ./hal_devices
HDW_PATH = ./hardware_dependent
WLD_PATH = ./world
GUI_PATH = ./gui

LIBHAL = $(HAL_PATH)/libhal.a
LIBHDW = $(HDW_PATH)/libhdw.a
LIBWLD = $(WLK_PATH)/libwld.a
LIBGUI = $(GUI_PATH)/libgui.a

ROBOLIB_PATH = ../robolib
ROBOLIB_IPATH = $(shell find $(ROBOLIB_PATH) -mindepth 2 -name Makefile | sed -e 's/\(^.*\)\/Makefile$$/-I\1/' | sed -e '/doc/d')

IPATH = -I$(HAL_PATH) -I$(HDW_PATH) -I$(WLD_PATH) -I$(GUI_PATH) -I../../PSoC/PSoC/shared -I. `pkg-config --cflags --libs gtk+-2.0` $(ROBOLIB_IPATH)

.PHONY: all
all: hdw hal wld gui $(OBJS)

.PHONY: hdw
hdw:
	make -C $(HDW_PATH)

.PHONY:hal
hal:
	make -C $(HAL_PATH)

.PHONY:wld
wld:
	make -C $(WLD_PATH)

.PHONY:gui
gui:
	make -C $(GUI_PATH)


.c.o:
	gcc -MD -c -o $@ -W -Wall $(IPATH) $<
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d

.PHONY: clean
clean:
	rm -f $(OBJS) *~ *.P
	make -C hardware_dependent clean
	make -C hal_devices clean

-include $(SRCS:.c=.P)
