CC = g++

CSRCS = $(shell ls *.c 2> /dev/null)
CXXSRCS = $(shell ls *.cpp 2> /dev/null)
OBJS = $(CSRCS:.c=.o) $(CXXSRCS:.cpp=.o)
HEADS = $(shell ls *.h 2> /dev/null) $(shell ls *.hpp 2> /dev/null)

CFLAGS = -g -O3 -Wall -std=c++0x -fopenmp -I/usr/include/vtk-5.8 -I/usr/include/pcl-1.7 -I/usr/include/eigen3 -I/usr/include/ni -I/usr/include/openni2  
#LFLAGS = -lm
LFLAGS = -lboost_system -lboost_filesystem -lboost_thread -lboost_date_time -lboost_iostreams -lboost_serialization -lboost_chrono -lpthread -lpcl_common -lOpenNI -lOpenNI2 /usr/lib/libvtkCommon.so.5.8.0 /usr/lib/libvtkFiltering.so.5.8.0 /usr/lib/libvtkImaging.so.5.8.0 /usr/lib/libvtkGraphics.so.5.8.0 /usr/lib/libvtkGenericFiltering.so.5.8.0 /usr/lib/libvtkIO.so.5.8.0 /usr/lib/libvtkRendering.so.5.8.0 /usr/lib/libvtkVolumeRendering.so.5.8.0 /usr/lib/libvtkHybrid.so.5.8.0 /usr/lib/libvtkWidgets.so.5.8.0 /usr/lib/libvtkParallel.so.5.8.0 /usr/lib/libvtkInfovis.so.5.8.0 /usr/lib/libvtkGeovis.so.5.8.0 /usr/lib/libvtkViews.so.5.8.0 /usr/lib/libvtkCharts.so.5.8.0 -lpcl_io -lboost_system -lboost_filesystem -lboost_thread -lboost_date_time -lboost_iostreams -lboost_serialization -lboost_chrono -lpthread -lpcl_common -lpcl_octree -lboost_system -lboost_filesystem -lboost_thread -lboost_date_time -lboost_iostreams -lboost_serialization -lboost_chrono -lpthread -lpcl_common -lOpenNI -lOpenNI2 -lpcl_io -lpcl_octree /usr/lib/libvtkViews.so.5.8.0 /usr/lib/libvtkInfovis.so.5.8.0 /usr/lib/libvtkWidgets.so.5.8.0 /usr/lib/libvtkVolumeRendering.so.5.8.0 /usr/lib/libvtkHybrid.so.5.8.0 /usr/lib/libvtkParallel.so.5.8.0 /usr/lib/libvtkRendering.so.5.8.0 /usr/lib/libvtkImaging.so.5.8.0 /usr/lib/libvtkGraphics.so.5.8.0 /usr/lib/libvtkIO.so.5.8.0 /usr/lib/libvtkFiltering.so.5.8.0 /usr/lib/libvtkCommon.so.5.8.0 -lm /usr/lib/libvtksys.so.5.8.0 -ldl

PROD = ./a.out

all: $(PROD)

$(PROD): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.SUFFIXES: .c .cpp .o
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
.cpp.o:
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJS): $(HEADS)

.PHONY: clean install
clean:
	rm -f $(PROD) $(OBJS)
