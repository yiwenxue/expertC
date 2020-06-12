CC=clang
LIBS=
INCLUDES= -I./
CFLAGS= -O3 ${INCLUDES} -DDebug=0 


all: mylist test

test: mylist
	@echo "Run test routine"
	./mylist 7

mylist: mylist.o
	$(CC) -o mylist mylist.o ${LIBS} ${CFLAGS}

mylist.o: main.c mylist.h
	$(CC) -c main.c -o mylist.o ${CFLAGS}

.PHONY:clean
clean: 
	rm *.o mylist
