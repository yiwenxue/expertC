CC=clang 
CFLAGS= -I./
LIBS=

project=namespace

src=main.c

all: ${project} test

test: 
	./${project}

${project}: ${src}
	${CC} ${CFLAGS} ${LIBS} -o ${project} ${src}

.PHONY:clean

clean:
	rm -rf ${project}
