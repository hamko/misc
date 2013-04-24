PKG = product
SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)

LIBHAL_PATH = ./2008_board_library/hal_devices
LIBHAL = $(LIBHAL_PATH)/libhal.a

LIBHDW_PATH = ./2008_board_library/hardware_dependent
LIBHDW = $(LIBHDW_PATH)/libhdw.a
STARTUP = $(LIBHDW_PATH)/startup.o
LINKER = $(LIBHDW_PATH)/linker.x

DM = ./2008_board_library/device_manager.o
HAL = ./2008_board_library/hal.o

ROBOLIB_PATH = ./robolib
ROBOLIB = $(ROBOLIB_PATH)/robolib.a

TOOLS_PREFIX = /usr/local/h8300-elf/bin/h8300-elf-
CC = $(TOOLS_PREFIX)gcc -O2
OBJCOPY = $(TOOLS_PREFIX)objcopy

IPATH = -I./2008_board_library -I$(LIBHAL_PATH) -I$(ROBOLIB_PATH)/heads

DEBUG = _N_DEBUG_

.PHONY: all
all: $(PKG).mot 

.PHONY:full
full: lib $(PKG).mot

.PHONY: lib
lib:
	make -C 2008_board_library
	make -C $(ROBOLIB_PATH)

$(PKG).mot: $(PKG)
	$(OBJCOPY) -O srec $< $@

$(PKG): $(HAL) $(DM) $(ROBOLIB) $(OBJS) $(LIBHAL) $(LIBHDW) $(STARTUP)
	$(CC) -W -Wall -o $@ -T $(LINKER) -nostartfiles -nostdlib -mh \
	$(STARTUP) $(OBJS) $(HAL) $(ROBOLIB) $(DM) $(LIBHDW) $(LIBHAL) \
	-u _int_wovi -lc -lgcc

.c.o:
	$(CC) -W -Wall $(IPATH) -D$(DEBUG) -o $@ -c -MD -mh $<
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d

$(ROBOLIB): $(LIBHAL)
	make -C $(ROBOLIB_PATH)

$(HAL): $(HAL:.o=.c) $(HAL:.o=.h)
	make -C 2008_board_library

$(DM): $(DM:.o=.c) $(DM:.o=.h)
	make -C 2008_board_library

.PHONY: write
write: $(PKG).mot
	h8write -3052 $(PKG).mot /dev/ttyUSB0

.PHONY: clean
clean:
	rm -f $(PKG)* $(OBJS) *~ *.P

.PHONY: full_clean
full_clean:
	make clean
	make -C $(ROBOLIB_PATH) clean
	make -C 2008_board_library clean

.PHONY: writeturbo
writeturbo: all
	./killtt.sh
	cygstart $(PKG).mot

-include $(SRCS:.c=.P)
