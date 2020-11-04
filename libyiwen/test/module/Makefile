CC= clang
CFLAGS= -I../ -I../../../libyiwen/include/
LIBS=  -ldl -L../../src/ -lyiwen
LDFLAGS= ${LIBS}

module= main

all:${module}
	@echo Module $< generated! 

${module}:${module}.o
	${CC} ${LDFLAGS} $< -o $@
	@ echo "remove trash"
	@ rm $<

${module}.o:${module}.c
	${CC} ${CFLAGS} -c $<

.PHONY: clean

clean: 
	rm -rf ${module} ${module}.o
