CC=clang
CXX=clang++

FLAGS=-O3

CFLAGS= -lpthread ${FLAGS}
CXXFLAGS= -lpthread  ${FLAGS}

all: pthreadpp pthread

pthread: pthread.c
	${CC} -o pthread pthread.c ${CFLAGS}

pthreadpp: pthreadpp.cpp
	${CXX} -o pthreadpp pthreadpp.cpp ${CXXFLAGS}

.PHONY:clean

clean:
	-rm -f *.o  pthread pthreadpp
