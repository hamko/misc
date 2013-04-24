INC_DIR= include
LIB_DIR= lib
BIN_DIR= .

LDFLAG=-L/usr/X11R6/lib# -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm -ljpeg
CFLAG= -O -I/usr/X11R6/include -DUSE_EYETOY -I/usr/src/linux-headers-2.6.32-41-generic/include -I/usr/src/linux-headers-2.6.32-41-generic/arch/x86/include #-I$(INC_DIR)

OBJS =
HEADDERS =

all: $(BIN_DIR)/simpleTest

$(BIN_DIR)/simpleTest: simpleTest.o $(OBJS)
	g++ -o $(BIN_DIR)/simpleTest simpleTest.o $(OBJS) $(LDFLAG) $(LIBS)

simpleTest.o: simpleTest.cpp $(HEADDERS)
	g++ -c $(CFLAG) simpleTest.cpp

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/simpleTest
