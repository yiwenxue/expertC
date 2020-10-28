CC=clang 
CFLAGS= -I./ -I./include
LIBS=

all: listen reverse_shell

listen: listen.c
	$(CC) ${CFLAGS} ${LIBS} listen.c -o listen

reverse_shell: reverse_shell.c
	$(CC) ${CFLAGS} ${LIBS} reverse_shell.c -o reverse_shell

.PHONY:clean

clean:
	rm -rf reverse_shell listen
