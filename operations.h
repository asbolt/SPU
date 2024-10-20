#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "stack.h"

#define FILE_NAME "text.txt"

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
    int codeSize = 0;
    Stack stack = {};
    int id = 0;
};

CMD operations (SPU *spu);
CMD assembler (SPU *spu, FILE *ptrFile);
int readFile (SPU *spu);

#endif