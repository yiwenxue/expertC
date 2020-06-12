CC=clang 
CFLAGS= -fPIC -I./
LIBS= -shared

all: module.ko

module.ko: module.o
	${CC} ${LIBS} -o module.ko module.o

module.o: module.c
	${CC} ${CFLAGS} -c module.c

.PHONY:clean

clean:
	rm -rf module.ko module.o
