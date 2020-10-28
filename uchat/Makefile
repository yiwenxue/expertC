CC=clang 
CFLAGS= -I./ -I./include -g 
LIBS= -lm -lpthread

all: server test

test: server 
	@echo "running server at 127.0.0.1 4444"
	./server 127.0.0.1 4444

server: server.c
	$(CC) ${CFLAGS} ${LIBS} server.c -o server

.PHONY:clean

clean:
	rm -rf server
