CC=clang
LIBS= gsl
CFLAGS= -std=c11 -Wall \
		-I${INCLUDE_DIR} \
		$(shell pkg-config --cflags ${LIBS})
LDFLAGS= -lm \
         -L${LIB_DIR} \
		 $(shell pkg-config --libs ${LIBS})

MKDIRP=mkdir -p
BUILD_DIR=${ROOT_DIR}/build
INCLUDE_DIR=${ROOT_DIR}/include
LIB_DIR=${ROOT_DIR}/lib

PERFSUIT_DIR=${ROOT_DIR}/third_part/FlameGraph/

PERF_DEP=${PERFSUIT_DIR}/flamegraph.pl\
	${PERFSUIT_DIR}/stackcollapse-perf.pl
