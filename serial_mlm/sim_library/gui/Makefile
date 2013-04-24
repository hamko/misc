SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)
HEADS = $(SRCS:.c=.h)
PKG = libgui

ROBOLIB_PATH = ../../robolib
ROBOLIB_IPATH = $(shell find $(ROBOLIB_PATH) -mindepth 2 -name Makefile | sed -e 's/\(^.*\)\/Makefile$$/-I\1/' | sed -e '/doc/d')

IFLAGS = -I.. -I../hal_devices -I ../world `pkg-config --cflags --libs gtk+-2.0` $(ROBOLIB_IPATH)

all: $(PKG).a

.c.o:
	gcc -W -Wall $(IFLAGS) -c -MD -o $@ $<
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d

$(PKG).a: $(OBJS)
	ar rcsv $@ $(OBJS)

.PHONY: multi_make
multi_make: $(patsubst %,$(OBJ_DIR)/%,$(OBJS)) $(patsubst %,$(HEAD_DIR)/%,$(HEADS))

$(OBJ_DIR)/%.o: %.o
	cp $< $@

$(HEAD_DIR)/%.h: %.h
	cp $< $@

.PHONY: clean
clean:
	rm -f $(OBJS) *~ *.P

-include $(SRCS:.c=.P)
