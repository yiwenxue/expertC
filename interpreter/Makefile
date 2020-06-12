CC=clang 
CFLAGS= -I./ 
LIBS= -ldl

project=inter

all: ${project} test

test: 
	@echo "Test ${project}:"
	./${project}

${project}: ${project}.c module.h list.h command.h 
	${CC} ${CFLAGS} ${LIBS} -o ${project} ${project}.c

.PHONY:clean

clean:
	rm -rf ${project}
