TARGET=getDeviceInfo

all: $(TARGET)

CFLAGS=-I /usr/local/cuda-5.0/include/ -I /usr/local/ArrayFire/include/ 
LFLAGS=-L /usr/local/ArrayFire/lib64/ -laf

getDeviceInfo: getDeviceInfo.cpp
	g++ -o $@ $(CFLAGS) $^ $(LFLAGS)

clean:
	rm *.o $(TARGET)
