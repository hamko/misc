PROD = ./a.out
HEADS = eigen.hpp
LIB = -lm
INC = -I/usr/local/include/eigen3/ 
OBJS = main.o

all: $(PROD)

$(PROD): $(OBJS)
	g++ $(INC) $^ -o $@ $(LIB)
	
.SUFFIXES: .cpp .o
.cpp.o:
	g++ -c $(INC) $^ -o $@ $(LIB)

.PHONY: clean
clean:
	-rm $(OBJS) $(PROD)
