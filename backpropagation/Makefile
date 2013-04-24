CC = gcc

OBJS = backpropagation.o nnsys.o
HEADS = nnsys.h
CFLAGS = -g -Wall

all: a.out

a.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lm

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $< -lm

$(OBJS): $(HEADS)

clean: 
	rm $(OBJS) a.out
