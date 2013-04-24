PRODUCT = ./a.out
OBJS = pf_sys.o pf_usr.o pf_sys_util.o pf_usr.o main.o io.o
HEADS = io.h params.h pf_usr.h pf_sys.h pf_sys_util.h
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

