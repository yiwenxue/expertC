CC= clang
CFLAGS= -I../include/
LIBS= 
LDFLAGS= ${LIBS} -ldl 

MODSRC= $(wildcard *.c)
# MODTMP= $(patsubst %.c, %.o, $(MODSRC))
MODKO= $(patsubst %.c, %, $(MODSRC))

all: ${MODKO}

%: %c
	${CC} ${CFLAGS} ${LDFLAGS} $< -o $@

.PHONY: clean

clean: 
	rm -rf ${MODKO}
