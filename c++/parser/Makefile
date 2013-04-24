PROD = ./a.out
CXX = g++
LFLAGS = -lm -lfl
CFLAGS = -g -Wall -Wno-sign-compare -Wno-parentheses
LEXFLAG = --header-file=lex.yy.hpp #-d

all: $(PROD)

$(PROD): main.o lex.yy.o 
	$(CXX) $(CFLAGS) -o $@ main.o lex.yy.o $(LFLAGS)

lex.yy.cpp : test.l
	flex -o lex.yy.cpp $(LEXFLAG) test.l

lex.yy.hpp : test.l
	flex -o lex.yy.cpp $(LEXFLAG) test.l

lex.yy.o : lex.yy.cpp
	$(CXX) -c $(CFLAGS) $< $(LFLAGS)

main.o : main.cpp lex.yy.hpp
	$(CXX) -c $(CFLAGS) $< $(LFLAGS)

.PHONY: clean
clean:
	-rm main.o lex.yy.cpp lex.yy.hpp
