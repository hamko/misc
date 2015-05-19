CC = g++

OBJS = simple.o
HEADS = 
CFLAGS = -g -Wall `xml2-config --cflags`
LFLAGS = `xml2-config --libs`

all: a.out

a.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS)

.SUFFIXES: .c .o .cpp
.c.o:
	$(CC) $(CFLAGS) -c $<  $(LFLAGS)
.cpp.o:
	$(CC) $(CFLAGS) -c $<  $(LFLAGS)

$(OBJS): $(HEADS)

clean: 
	rm $(OBJS) a.out
