CFLAGS = -g -Wall
LFLAGS = -lm
all: a.out


a.out: k-means.c k-means.h main.c
	gcc $(CFLAGS) $^ $(LFLAGS)
