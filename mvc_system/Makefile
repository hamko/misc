PROD = a.out
CXX = g++
CFLAGS = -Wall -g
LFLAGS = -lm
SRCS = $(shell ls *.cpp)
HEADS = $(shell ls *.hpp)
OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .cpp .o
	$(CXX) -c $(CFLAGS) -o $@ $^ $(LFLAGS)

all : $(PROD)

$(PROD) : $(OBJS) $(HEADS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: clean
clean:
	-rm *.o $(PROD)
