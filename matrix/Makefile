OBJS = main.o matrix.o matrix_test.o
HEADS = matrix.h matrix_test.h
CFLAGS = -g -Wall
LFLAGS = -lm
PROD = a.out

all: a.out

$(PROD): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $< $(LFLAGS)

$(OBJS): $(HEADS)

.PHONY: clean
clean:
	rm $(PROD) $(OBJS)
