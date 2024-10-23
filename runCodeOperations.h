#ifndef RUN_CODE_OPERATIONS_H_
#define RUN_CODE_OPERATIONS_H_

#include "spu.h"
#include "sign.h"

#define FILE_NAME "program_code.txt"
#define ASM_FILE  "program.asm"
#define SIGN      "ABED"
#define VERSION   2


int run (SPU *spu);
int makeCode (SPU *spu, FILE *ptrFile);
int codeCtor (int size, SPU *spu);

#endif 