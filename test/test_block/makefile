
EXE_NAME = TEST_BLOCK_EXE

CXXFLAGS += -Wall
CXXFLAGS += -g

LDFLAGS += -lgtest_main 
LDFLAGS += -lgtest 
LDFLAGS += -lpthread

DEBUG_FLAG += -DEBUG

INC =
INC += -I../../include

SRC = 
SRC += ../../lib/block
SRC += ../../lib/log

CXX = g++

MAIN_CPP = ./test_block.cpp

SRC_FILES = ${addsuffix .c, ${SRC}}
OBJ_FILES = ${addsuffix .o, ${SRC}}

all:${EXE_NAME} 

${EXE_NAME}:${OBJ_FILES}
	${CXX} ${CXXFLAGS} ${INC} ${OBJ_FILES} ${MAIN_CPP} -o $@ ${LDFLAGS}

${OBJ_FILES}: %.o : %.c
	${CXX} ${CXXFLAGS} -c ${INC} $< -o $@ ${LDFLAGS} ${DEBUG_FLAG}
clean:
	rm -rf ${OBJ_FILES} ${EXE_NAME} filesystem.txt *.log

