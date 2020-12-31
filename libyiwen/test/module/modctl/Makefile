CC= clang
CFLAGS= -fPIC -I../ -I../../../libyiwen/include/
LIBS=  -ldl -L../../../lib/ -lyiwen
LDFLAGS= -shared ${LIBS}

module=modctl

all:${module}.ko
	@echo Module $< generated! 

${module}.ko:${module}.o
	${CC} ${LDFLAGS} $< -o $@
	@ echo "remove trash"
	@ rm $<

${module}.o:${module}.c
	${CC} ${CFLAGS} -c $<

.PHONY: clean

clean: 
	rm -rf ${module}.ko
