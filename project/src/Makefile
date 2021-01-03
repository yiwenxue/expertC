include ../config.mk

src= main.c
header= ${INCLUDE_DIR}/fib.h

target=${BUILD_DIR}/main

all: details ${target}
	@echo "[Done!]"

details:
	@echo "[Build Project details]"
	@echo "      CC      : ${CC}"
	@echo "      ROOT_DIR: ${ROOT_DIR}"
	@echo "      LIBS    : ${LIBS}"
	@echo "      CFLAGS  : ${CFLAGS}"
	@echo "      LDFLAGS : ${LDFLAGS}"
	@echo      

${target}: ${src} ${header}
	@echo "    ${MKDIRP} ${BUILD_DIR}"
	@${MKDIRP} ${BUILD_DIR}
	@echo "    ${CC} ${src} ${CFLAGS} ${LDFLAGS} -o ${target}"
	@${CC} ${src} ${CFLAGS} ${LDFLAGS} -o ${target}

perf: ${target}\
	${PERF_DEP}
	@echo "    perf record -F 99 -g -- ${target}"
	@perf record -F 99 -g -- ${target}
	@echo "    perf script | ${PERFSUIT_DIR}/stackcollapse-perf.pl > ${target}.perf-folded"
	@perf script | ${PERFSUIT_DIR}/stackcollapse-perf.pl > ${target}.perf-folded
	@echo "    ${PERFSUIT_DIR}/flamegraph.pl ${target}.perf-folded > ${target}-perf.svg"
	@${PERFSUIT_DIR}/flamegraph.pl ${target}.perf-folded > ${target}-perf.svg

.PHONY: clean

clean: 
	@echo "    rm -rf ${target}"
	@rm -rf ${target}
	@echo "    rm -rf ${target}.perf-folded ${target}-perf.svg"
	@rm -rf ${target}.perf-folded ${target}-perf.svg
