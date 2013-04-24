OBJS = rsys.o main.o
PKG = a.out

a.out: ${OBJS}
	${CC} -g -o $@ $^ -lm

SUFFIXES: .c .o
.c.o: 
	${CC} -c -g $^ -lm
