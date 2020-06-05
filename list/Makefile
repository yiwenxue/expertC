CC=clang
LIBS=
INCLUDES= -I./
CFLAGS= -O3 ${INCLUDES}


all: mylist 

mylist: mylist.o
	$(CC) -o mylist mylist.o ${LIBS} ${CFLAGS}

mylist.o: main.c mylist.h
	$(CC) -c main.c -o mylist.o ${CFLAGS}

.PHONY:clean
clean: 
	rm *.o mylist
