SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean
all: $(OBJS) $(patsubst %,$(ROBOLIB_PATH)/heads/%,$(shell ls *.h))

.c.o:
	$(CC) $(CFLAGS) -D$(subst .,_,$<) -c $< -o $@
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d
	$(AR) $(ARFLAGS) $(ROBOLIB) $@

$(ROBOLIB_PATH)/heads/%.h: %.h
	cp $< $@

clean:
	rm -f $(OBJS) *~ *.P

-include $(SRCS:.c=.P)
