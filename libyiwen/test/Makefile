CC= clang
CFLAGS= -I../include/
LIBS= 
LDFLAGS= ${LIBS}

SRC= $(wildcard *.c)
MODKO= $(patsubst %.c, %, $(SRC))

all: ${MODKO}

%: %.c
	${CC} ${CFLAGS} ${LDFLAGS} $< -o $@

.PHONY: clean

clean: 
	rm -rf ${MODKO}

