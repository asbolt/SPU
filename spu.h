#ifndef SPU_H_
#define SPU_H_

#include "stack.h"
#include "stackUserInterface.h"
#include <cstring>

#define ASM_FILE "program_code.txt"
#define BINARY_FILE "program.asm"
#define SIGN "ABED"
#define VERSION 2
#define DEFAULT_STACK_SIZE 10

const int sizeLabel = 15;
const int numberLabels = 10;
const int DEFAULT_REG_SIZE = 4;
const int DEFAULT_RAM_SIZE = 100;
const int basisNumberSystem = 10;

enum ERROR_CODES
{
    CORRECT_ = 0,
    ERROR_ = 1
};

enum REGS
{
    AX = 1,
    BX = 2,
    CX = 3
};

enum PUSH_MODE
{
    ARG             = 1,
    REG             = 2,
    RAM_ARG         = 3,
    RAM_REG         = 4,
    RAM_ARG_AND_REG = 5
};

enum CMD
{
    PUSH  = 1,
    POP   = 2,
    ADD   = 3,
    SUB   = 4,
    MUL   = 5,
    DIV   = 6,
    OUT   = 7,
    JUMP  = 11,
    JA    = 12,
    JB    = 13,
    LABEL = 21,
    HLT   = -1,
    ERROR = -2
};

struct Labels 
{
    char **labelName;
    int *labelIp;
    int labelNumber = 0;
};

struct SPU
{
    int *code      = NULL;
    Stack stack    = {};
    int *regs      = NULL;
    Labels labels  = {};
    int *RAM       = NULL;
    int ip         = 0;
};

ERROR_CODES labelCtor (Labels *labels);
ERROR_CODES labelDtor (Labels *labels);
ERROR_CODES codeArrayCtor (int size, SPU *spu);
ERROR_CODES codeArrayDtor (int *code);
ERROR_CODES spuCtor (SPU *spu, int size);
ERROR_CODES spuDtor (SPU *spu);


#endif