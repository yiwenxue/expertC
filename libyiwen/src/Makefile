CC= clang
CFLAGS= -I../include/ -fPIC
LIBS= -ldl 
LDFLAGS= ${LIBS} -shared

incyiwen= ../include/log.h\
		  ../include/list.h\
		  ../include/module.h\
		  ../include/command.h\
		  ../include/yiwen_int.h\
		  ../include/yiwen_fatal.h\
		  ../include/yiwen_string.h\
		  ../include/yiwen_memview.h\
		  ../include/yiwen_pcolor.h

srcyiwen= ./module.c \
		  ./command.c \
		  ./yiwen_int.c

tempyiwen= $(patsubst %.c, %.o, $(srcyiwen))

all: libyiwen.so

libyiwen.so: ${tempyiwen}
	${CC} ${LDFLAGS} ${tempyiwen} -o libyiwen.so
	rm ${tempyiwen}

module.o: ${incyiwen} module.c
	${CC} ${CFLAGS} -c module.c -o module.o

command.o: ${incyiwen} command.c
	${CC} ${CFLAGS} -c command.c -o command.o

yiwen_int.o: ${incyiwen} yiwen_int.c
	${CC} ${CFLAGS} -c yiwen_int.c -o yiwen_int.o


.PHONY: clean

clean: 
	rm -rf libyiwen.so ${tempyiwen}
