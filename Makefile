CC?=gcc

BIN_DIR=${PWD}/bin
O_DIR=${BIN_DIR}

CFLAGS=-O2 -std=gnu11 -I ./include
LDFLAGS=-lm -pthread -ltensorflow -L${BIN_DIR} -lutils -lm

.PHONY: default
default: main

main: mkdir ${BIN_DIR}/fantastic-filter-cli

dev: CFLAGS:=$(CFLAGS) -g -fsanitize=undefined -fsanitize=leak
dev: LDFLAGS:=$(LDFLAGS) -lubsan
dev: ${BIN_DIR}/debug


test: CFLAGS:=$(CFLAGS) -g -fsanitize=undefined
test: LDFLAGS:=$(LDFLAGS) -lubsan
test: ${BIN_DIR}/test
	${BIN_DIR}/test
${BIN_DIR}/test: ${BIN_DIR}/FF.o ${BIN_DIR}/args.o utils src/main.c src/process.c test/*.c
	${CC} test/*.c src/benchmark.c src/process.c ${BIN_DIR}/FF.o ${BIN_DIR}/args.o  -o ${BIN_DIR}/test  ${CFLAGS} ${LDFLAGS}

${BIN_DIR}/fantastic-filter-cli: ${BIN_DIR}/FF.o ${BIN_DIR}/args.o utils src/main.c src/process.c src/benchmark.c
	${CC} src/main.c src/benchmark.c src/process.c ${BIN_DIR}/FF.o ${BIN_DIR}/args.o  -o ${BIN_DIR}/fantastic-filter-cli  ${CFLAGS} ${LDFLAGS}

${BIN_DIR}/debug: ${BIN_DIR}/FF.o ${BIN_DIR}/args.o utils src/main.c src/process.c  src/benchmark.c
	${CC} src/main.c src/benchmark.c src/process.c ${BIN_DIR}/FF.o ${BIN_DIR}/args.o  -o ${BIN_DIR}/debug  ${CFLAGS} ${LDFLAGS}
	

${BIN_DIR}/FF.o: src/FF.c
	${CC} -c src/FF.c -o ${BIN_DIR}/FF.o ${CFLAGS}


${BIN_DIR}/args.o: src/args.c utils
	${CC} -c src/args.c -o ${BIN_DIR}/args.o ${CFLAGS}


clean:
	rm ${BIN_DIR}/* -rf

mkdir: ${BIN_DIR}

${BIN_DIR}:
	mkdir -p ${BIN_DIR}/

utils: ${BIN_DIR}/libutils.a

${O_DIR}/utils/%.o: src/utils/%.c
	mkdir -p ${O_DIR}/utils
	$(CC) -c -o $@ $< ${CFLAGS}


${BIN_DIR}/libutils.a: ${O_DIR}/utils/vector.o ${O_DIR}/utils/jpg_info.o
	ar rcs ${BIN_DIR}/libutils.a ${O_DIR}/utils/vector.o ${O_DIR}/utils/jpg_info.o

