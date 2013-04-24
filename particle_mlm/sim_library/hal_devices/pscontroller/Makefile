SRCS = $(shell ls *.c)
HEADS = $(SRCS:.c=.h)
PHEADS = $(SRCS:.c=_prv.h)
OBJS = $(SRCS:.c=.o)

TOOLS_PREFIX = /usr/bin/
CC = $(TOOLS_PREFIX)gcc
IFLAGS= -I../..

.PHONY: all
all: $(OBJS)

.c.o:
	$(CC) -W -Wall $(IFLAGS) -c -MD -o $@ $<
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d

.PHONY: multi_make
multi_make: $(patsubst %,$(OBJ_DIR)/%,$(OBJS)) $(patsubst %,$(HEAD_DIR)/%,$(HEADS) $(PHEADS))

$(OBJ_DIR)/%.o: %.o
	cp $< $@

$(HEAD_DIR)/%.h: %.h
	cp $< $@

.PHONY: clean
clean:
	rm -f $(OBJS) *~ *.P

-include $(SRCS:.c=.P)
