TARGETS = smallest comparison
all: $(TARGETS)

CFLAGS=-g -O2 -std=c++0x -fopenmp
LFLAGS=-ltbb -lm
smallest: smallest.cpp
	g++ $(CFLAGS) -o $@ $^ $(LFLAGS)
comparison:comparison.cpp
	g++ $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm $(TARGETS)
