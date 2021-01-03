include ./config.mk
export ROOT_DIR=$(shell pwd)

project=main

perf=perf

test=testsuit

all: ${project}

${project}: src/Makefile
	@${MAKE} -C src

${perf}: src/Makefile \
		test/Makefile 
	@${MAKE} -C src perf
	@${MAKE} -C test perf

${test}: test/Makefile
	@${MAKE} -C test

build: ${project} ${perf} ${test}

.PHONY: clean 

clean:
	@${MAKE} -C src clean
	@${MAKE} -C test clean
	@rm -rf ${BUILD_DIR}
