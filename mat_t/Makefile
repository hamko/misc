PKG = a.out
OBJS = robolib_matrix.o main.o dfree.o

all: $(PKG)

$(PKG): $(OBJS)
	gcc -g -o $@ $^ -lm

SUFFIXES: .c .o
.c.o:
	$(CC) -c -g $^ -lm

robolib_matrix.o: robolib_matrix.h
dfree.o: dfree.h
