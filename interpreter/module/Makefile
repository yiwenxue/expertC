CC= clang
CFLAGS= -fPIC -I../ -I../../libyiwen/include/
LIBS= 
LDFLAGS= -shared ${LIBS}

MODSRC= $(wildcard *.c)
# MODTMP= $(patsubst %.c, %.o, $(MODSRC))
MODKO= $(patsubst %.c, %.ko, $(MODSRC))

all: ${MODKO}

%.ko: %.o
	${CC} ${LDFLAGS} $< -o $@
	@echo Generate module $@
	@echo ""

%.o: %c
	${CC} ${CFLAGS} $< 

.PHONY: clean

clean: 
	rm -rf ${MODKO}
