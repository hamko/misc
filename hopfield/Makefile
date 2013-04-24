CC = gcc
LFLAGS = -g
CFLAGS = -g

OBJS = hopfield.o robolib_matrix.o
all: a.out

a.out: $(OBJS)
	$(CC) -o $@ $(OBJS) -lm

hopfield.o: hopfield.c

robolib_matrix.o: robolib_matrix.c robolib_matrix.h

clean: 
	rm a.out hopfield.o robolib_matrix.o
