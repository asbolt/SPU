#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "stack.h"

#define FILE_NAME "program_code.txt"
#define ASM_FILE "program.asm"

enum CMD
{
    PUSH = 1,
    POP = 2,
    ADD = 3,
    SUB = 4,
    MUL = 5,
    DIV = 6,
    OUT = 7,
    HLT = -1,
    ERROR = -2
};

struct SPU
{
    int code [25] = {};
    Stack stack = {};
    int regs [4] = {};
    int RAM [100] = {};
    int ip = 0;
};

int operations (SPU *spu);
int makeCode (SPU *spu, FILE *ptrFile);
int readFile (SPU *spu);
CMD assembler (FILE *ptrFile, FILE *asmFile);

#endif