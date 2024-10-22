#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "stack.h"

#define FILE_NAME "program_code.txt"
#define ASM_FILE "program.asm"
#define SIGN "ABED"
#define VERSION 2

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

struct Label
{
    char name [15] = {};
    int ip = 0;
};

struct SPU
{
    int *code = NULL;
    Stack stack = {};
    int regs [4] = {};
    Label labels [10] = {};
    int RAM [100] = {};
    int ip = 0;
};

int operations (SPU *spu);
int makeCode (SPU *spu, FILE *ptrFile);
int readFile (SPU *spu);
CMD assembler (FILE *ptrFile, FILE *asmFile);
int sign (FILE *asmFile, FILE *ptrFile);
int checkSign (FILE *asmFile);
int codeCtor (int size, SPU *spu);

#endif 