CXX 		= g++
CXXLAGS 	= -Wall
SRC_DIRS	= ./src
LIBS		= ./include
OUT_DIRS	= ./out
BENCHMARK_DIRS = ./benchmark

.PHONY: main
main:
	${CXX} ${CXXLAGS} ${LIBS}/*.cpp ${SRC_DIRS}/m11207805.cpp -o synthesis

.PHONY: test
test:
	${CXX} ${CXXLAGS} ${LIBS}/*.cpp ${SRC_DIRS}/test.cpp -o test

.PHONY: clean
clean:
	rm -rf synthesis test
	find ${OUT_DIRS} -type f -name "*.blif" -delete
	find . -type f -name "*.log"  -delete