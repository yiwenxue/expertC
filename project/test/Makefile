include ../config.mk

LIBS+= cunit

TEST_DIR=${BUILD_DIR}/test

SRC=$(wildcard *.c)
TARGET=$(patsubst %.c,${TEST_DIR}/%, $(SRC))
PERF_TARGET=$(patsubst %.c,${TEST_DIR}/%-perf.svg, $(SRC))
PERF_TMP=$(patsubst %.c,${TEST_DIR}/%.perf-folded, $(SRC))
HEADER=${INCLUDE_DIR}/fib.h


all: details ${TARGET}
	@echo "[Done!]"

details:
	@echo "[Build Tests details]"
	@echo "      CC      : ${CC}"
	@echo "      ROOT_DIR: ${ROOT_DIR}"
	@echo "      LIBS    : ${LIBS}"
	@echo "      CFLAGS  : ${CFLAGS}"
	@echo "      LDFLAGS : ${LDFLAGS}"
	@echo      

${TEST_DIR}/%: %.c ${HEADER}
	@echo "    ${MKDIRP} ${TEST_DIR}"
	@${MKDIRP} ${TEST_DIR}
	@echo "    ${CC} ${CFLAGS} ${LDFLAGS} $< -o $@"
	@${CC} ${CFLAGS} ${LDFLAGS} $< -o $@

perf: ${TARGET} ${PERF_TARGET}

${TEST_DIR}/%-perf.svg: %.c ${PERF_DEP}
	@echo "Hey "$(patsubst %.c,${TEST_DIR}/%, $<)
	@echo "Hey "$@
	@echo "    perf record -F 99 -g -- $(patsubst %.c,${TEST_DIR}/%, $<)"
	@perf record -F 99 -g -- $(patsubst %.c,${TEST_DIR}/%, $<)
	@echo "    perf script | ${PERFSUIT_DIR}/stackcollapse-perf.pl > $(patsubst %.c,${TEST_DIR}/%, $<).perf-folded"
	@perf script | ${PERFSUIT_DIR}/stackcollapse-perf.pl > $(patsubst %.c,${TEST_DIR}/%, $<).perf-folded
	@echo "    ${PERFSUIT_DIR}/flamegraph.pl $(patsubst %.c,${TEST_DIR}/%, $<).perf-folded > $@"
	@${PERFSUIT_DIR}/flamegraph.pl $(patsubst %.c,${TEST_DIR}/%, $<).perf-folded > $@

.PHONY: clean

clean: 
	@echo "    rm -rf ${TARGET}"
	@rm -rf ${TARGET}
	@echo "    rm -rf ${TEST_DIR}"
	@rm -rf ${TEST_DIR}
	@echo "    rm -rf ${PERF_TMP}"
	@rm -rf ${PERF_TMP}
	@echo "    rm -rf ${PERF_TARGET}"
	@rm -rf ${PERF_TARGET}
