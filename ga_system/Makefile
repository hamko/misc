PRODUCT = gasys
OBJS = ga_sys.o ga_usr.o main.o io.o ga_usr_utils.o
HEADS = io.h params.h ga_usr.h ga_usr_utils.h ga_sys.h
CFLAGS = -g -Wall
LFLAGS = -lm

all: $(PRODUCT)

.SUFFIXES: .c .o
.c.o: 
	gcc $(CFLAGS) -o $@ -c $< $(LFLAGS)

$(OBJS): $(HEADS)

$(PRODUCT): $(OBJS)
	gcc $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: clean
clean:
	rm $(PRODUCT) $(OBJS)

