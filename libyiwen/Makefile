CC= clang
CFLAGS= -I/include/ -fPIC
LIBS= -ldl 
LDFLAGS= ${LIBS} -shared

incyiwen= ./include/log.h\
		  ./include/list.h\
		  ./include/module.h\
		  ./include/command.h\
		  ./include/yiwen_int.h\
		  ./include/yiwen_fatal.h\
		  ./include/yiwen_string.h\
		  ./include/yiwen_memview.h\
		  ./include/yiwen_pcolor.h

srcyiwen= ./src/module.c \
		  ./src/command.c

tempyiwen= $(patsubst %.c, %.o, $(srcyiwen))

library=lib/libyiwen.so
all: library

library: ${tempyiwen}
	mkdir -p lib 
	${CC} ${LDFLAGS} ${tempyiwen} -o lib/libyiwen.so
	rm ${tempyiwen}

module.o: ${incyiwen} ./src/module.c
	${CC} ${CFLAGS} -c ./src/module.c -o ./src/module.o

command.o: ${incyiwen} ./src/command.c
	${CC} ${CFLAGS} -c ./src/command.c -o ./src/command.o

.PHONY: clean

clean: 
	rm -rf lib/libyiwen.so ${tempyiwen}

