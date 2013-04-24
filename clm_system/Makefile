PRODUCT = clmsys
OBJS = clm_sys.o clm_usr.o main.o io.o clm_usr_utils.o
HEADS = io.h params.h clm_usr.h clm_sys.h clm_usr_utils.h
CFLAGS = -g -Wall
LFLAGS = -lm

all: $(PRODUCT)

.SUFFIXES: .c .o
.c.o: 
	gcc $(CFLAGS) -o $@ -c $< $(LFLAGS)

$(OBJS): $(HEADS)

$(PRODUCT): $(OBJS)
	gcc $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: clean
clean:
	rm $(PRODUCT) $(OBJS)

