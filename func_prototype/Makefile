CC=gcc
CFLAGS=
LIBS=

project=func_prototype

all: ${project} test

test: 
	./${project} hello\ world

${project}: ${project}.c
	${CC} ${CFLAGS} ${LIBS} -o ${project} ${project}.c

.PHONY:clean

clean:
	rm -rf ${project}
