PROD = ./a.out
OBJS = monopoly.o polyinfrastructure.o poly.o main.o wu_method.o
HEADS = monopoly.hpp polyinfrastructure.hpp number.hpp poly.hpp util.hpp operand.hpp wu_method.hpp
CXX = g++
LFLAGS = -lm
CFLAGS = -g -Wall -Wno-sign-compare

all: $(PROD)

$(PROD): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LFLAGS)

.SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) -c $(CFLAGS) $< $(LFLAGS)

.PHONY: clean
clean:
	-rm $(OBJS) $(PROD)

$(OBJS) : $(HEADS)
