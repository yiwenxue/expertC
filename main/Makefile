CC=clang 
CFLAGS= -I./
LIBS=

project=main

all: ${project} test

test: 
	./${project} -c conf

${project}: ${project}.c
	${CC} ${CFLAGS} ${LIBS} -o ${project} ${project}.c

.PHONY:clean

clean:
	rm -rf ${project}
