CC=clang 
CFLAGS=
LIBS=

project=strncmp

all: ${project} test

test: 
	./${project} string1 string2 6
	./${project} string1 string2 7

${project}: ${project}.c
	${CC} ${CFLAGS} ${LIBS} -o ${project} ${project}.c

.PHONY:clean

clean:
	rm -rf ${project}
