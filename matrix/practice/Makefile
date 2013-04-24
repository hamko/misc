PROD = question1 question2
MATRIXFILES = ../matrix.c ../matrix.h
MATRIXDIR = ../

all: $(PROD)

question1: question1.c  $(MATRIXFILES)
	gcc -g -Wall -o $@ $^ -I$(MATRIXDIR) -lm

question2: question2.c question2_another.c question2_another.h $(MATRIXFILES)
	gcc -g -Wall -o $@ $^ -I$(MATRIXDIR) -lm

.PHONY: clean
clean:
	rm question1 question2 question1.o question2.o question2_another.o
