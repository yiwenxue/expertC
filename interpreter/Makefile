CC=clang 
CFLAGS=-I./ -I../libyiwen/include/
LIBS= -ldl

project=inter

all: ${project} test

modules: ./module/Makefile
	make -C module/ -f Makefile 

test: 
	@echo "Test ${project}:"
	./${project}

${project}: ${project}.c command.h 
	${CC} ${CFLAGS} ${LIBS} -o ${project} ${project}.c

.PHONY:clean

clean:
	rm -rf ${project}
	make -C module/ -f Makefile clean 
