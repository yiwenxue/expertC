CC = clang 
CFLAGS= -c -g 
LDFLAGS= 
LIBS= 

project=rc 

all: rc

rc:rc.o
	$(CC) $(LDFLAGS) rc.o -o rc

rc.o: main.c 
	$(CC) $(CFLAGS) main.c -o rc.o

.PHONY: clean
clean:
	rm rc *.o
