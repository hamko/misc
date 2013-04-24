SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)
HEADS = $(SRCS:.c=.h)

IFLAGS = -I../.. -I../../hal_devices -I../../world

.PHONY: all
all: $(OBJS)

.c.o:
	gcc -W -Wall $(IFLAGS) -c -MD -o $@ $<
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d

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
