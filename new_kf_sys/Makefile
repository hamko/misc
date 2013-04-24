PKG = a.out
OBJS = robolib_matrix.o matrix.o main.o rsys.o newkfsys.o dfree.o
HEADS = robolib_matrix.h matrix.h rsys.h newkfsys.h dfree.h option.h

all: $(PKG)

a.out: $(OBJS)
	$(CC) -g -o $@ $^ -lm

SUFFIXES: .c .o
.c.o:
	$(CC) -g -c $^ -lm

$(OBJS): $(HEADS)

clean: 
	rm $(OBJS) $(PKG) *.gch

