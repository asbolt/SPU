CC = g++

all: asm

asm: assembler.cpp runCodeOperations.cpp sign.cpp spu.cpp stackError.cpp stackInsideFunctoins.cpp stackUserInterface.cpp mainAsm.cpp
	$(CC) assembler.cpp runCodeOperations.cpp sign.cpp spu.cpp stackError.cpp stackInsideFunctoins.cpp stackUserInterface.cpp mainAsm.cpp

code: assembler.cpp runCodeOperations.cpp sign.cpp spu.cpp stackError.cpp stackInsideFunctoins.cpp stackUserInterface.cpp mainRunCode.cpp
	$(CC) assembler.cpp runCodeOperations.cpp sign.cpp spu.cpp stackError.cpp stackInsideFunctoins.cpp stackUserInterface.cpp mainRunCode.cpp
