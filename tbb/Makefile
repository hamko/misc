TARGETS = smallest comparison comparison_not_work parallel_reduce
all: $(TARGETS)

CFLAGS=-g -O2 -std=c++0x -fopenmp
LFLAGS=-ltbb -lm
smallest: smallest.cpp
	g++ $(CFLAGS) -o $@ $^ $(LFLAGS)
comparison:comparison.cpp
	g++ $(CFLAGS) -o $@ $^ $(LFLAGS)
parallel_reduce:parallel_reduce.cpp
	g++ $(CFLAGS) -o $@ $^ $(LFLAGS)
comparison_not_work:comparison_not_work.cpp
	g++ $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm $(TARGETS)
