SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)
HEADS = $(SRCS:.c=.h)   # ヘッダは.cと名前が対応していなければならない
PKG = libwld

IFLAGS = -I../ -I../hal_devices

.PHONY: all
all: $(OBJS) $(PKG).a $(PKG).h

.c.o:
	gcc -W -Wall $(IFLAGS) -c -MD -o $@ $<
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d

$(PKG).h: $(HEADS)
	echo "#ifndef _LIBWRD_H_" > $@
	echo "#define _LIBWRD_H_" >> $@
	@for inc in $(shell ls *.h); \
		do (echo "#include\""$$inc"\"" >> $@); \
	done
	echo "#endif //_LIBWRD_H_" >> $@

$(PKG).a: $(OBJS)
	ar rcsv $@ $(OBJS)

.PHONY: multi_make
multi_make: $(patsubst %,$(OBJ_DIR)/%,$(OBJS)) $(patsubst %,$(HEAD_DIR)/%,$(HEADS))

.PHONY: clean
clean:
	rm -f $(OBJS) *~ *.P

-include $(SRCS:.c=.P)
