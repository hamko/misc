TARGET=getDeviceInfo matmul matrixComparison

all: $(TARGET)

CFLAGS=-fopenmp -O3 -I/usr/local/cuda-5.0/include/ -I/usr/local/ArrayFire/include/ -I/usr/include/eigen3
LFLAGS=-L/usr/local/ArrayFire/lib64/ -laf

getDeviceInfo: getDeviceInfo.cpp
	g++ -o $@ $(CFLAGS) $^ $(LFLAGS)
matmul: matmul.cpp
	g++ -o $@ $(CFLAGS) $^ $(LFLAGS)
matrixComparison: matrixComparison.cpp
	g++ -o $@ $(CFLAGS) $^ $(LFLAGS)

clean:
	rm *.o $(TARGET)
