CC=clang 
CFLAGS= -I./ -I./include
LIBS=

project=pipe

all: ${project} test

test: 
	./${project}

${project}: ${project}.c
	${CC} ${CFLAGS} ${LIBS} -o ${project} ${project}.c

.PHONY:clean

clean:
	rm -rf ${project}
