TARGETS = smallest
all: $(TARGETS)

CFLAGS=-g -O2 -std=c++0x
LFLAGS=-ltbb
smallest: smallest.cpp
	g++ $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm $(TARGETS)
