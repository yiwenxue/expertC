CC=clang 
CFLAGS=
LIBS=

project=args

all: ${project} test

test: 
	./${project} -f args.c

${project}: ${project}.c
	${CC} ${CFLAGS} ${LIBS} -o ${project} ${project}.c

.PHONY:clean

clean:
	rm -rf ${project}
