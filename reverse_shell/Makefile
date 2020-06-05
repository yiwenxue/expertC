CC=clang
CFLAGS=

all: client server 

client: client.c
	$(CC) client.c -o client

server: server.c 
	$(CC) server.c -o server 


.PHONY: clean

clean:
	rm -f server client 
