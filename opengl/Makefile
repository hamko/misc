#MAKEFILE

CC=g++
RM=rm -f

CFLAGS=-Wall
LDFLAGS=-L/usr/local/lib -lglut -lGL -lGLU -ljpeg -lm -lpthread -lhamkoutil

TARGET = display_callback keyboard_callback teapot robot_arm planet basic tex_teapot image texbind point pthread

all: $(TARGET)

.c.o:
	${CC} -c ${CFLAGS} $< ${LDFLAGS}

display_callback: display_callback.o
	${CC} -o $@ $^ ${LDFLAGS}

keyboard_callback: keyboard_callback.o
	${CC} -o $@ $^ ${LDFLAGS}

teapot: teapot.o
	${CC} -o $@ $^ ${LDFLAGS}

tex_teapot: tex_teapot.o
	${CC} -o $@ $^ ${LDFLAGS}

robot_arm: robot_arm.o
	${CC} -o $@ $^ ${LDFLAGS}

planet: planet.o
	${CC} -o $@ $^ ${LDFLAGS}

basic: basic.o
	${CC} -o $@ $^ ${LDFLAGS}
	
image: image.o readjpeg.o readjpeg.h
	${CC} -o $@ $^ ${LDFLAGS}

texbind: texbind.o readjpeg.o readjpeg.h
	${CC} -o $@ $^ ${LDFLAGS}

point: point.o 
	${CC} -o $@ $^ ${LDFLAGS}

pthread: pthread.o pthread_gl.o
	${CC} -o $@ $^ ${LDFLAGS}

clean:
	${RM} ${TARGET} *.o *~

