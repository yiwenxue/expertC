CC=clang 
CFLAGS= -I./
LIBS= -ldl 

project=loadable

all: ${project} module test

test: 
	@echo "Test ${project}:"
	./${project} ./module.ko

${project}: ${project}.c
	${CC} ${CFLAGS} ${LIBS} -o ${project} ${project}.c

module: module.c Makefile.m 
	make -C ./ -f Makefile.m

.PHONY:clean

clean:
	rm -rf ${project}
	make -C ./ -f Makefile.m clean

