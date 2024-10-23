#ifndef SPU_H_
#define SPU_H_

#include "stack.h"
#include "stackUserInterface.h"
#include <cstring>

#define FILE_NAME "program_code.txt"
#define ASM_FILE "program.asm"
#define SIGN "ABED"
#define VERSION 2

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
    char labelName [10][15] = {};
    int labelIp [10]        = {};
    int labelNumber         = 0;
};

struct SPU
{
    int *code     = NULL;
    Stack stack   = {};
    int regs [4]  = {};
    Labels labels = {};
    int RAM [100] = {};
    int ip        = 0;
};

#endif