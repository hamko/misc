PROD = ./a.out
OBJS = operator.o main.o
HEADS = operator.hpp
CXX = g++
LFLAGS = -lm
CFLAGS = -g -Wall -Wno-sign-compare

all: $(PROD)

$(PROD): $(OBJS) $(HEADS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LFLAGS)

.SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) -c $(CFLAGS) $< $(LFLAGS)

.PHONY: clean
clean:
	-rm $(OBJS) $(PROD)
