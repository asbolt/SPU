#ifndef RUN_CODE_OPERATIONS_H_
#define RUN_CODE_OPERATIONS_H_

#include "spu.h"
#include "sign.h"

int runCodeCommand (SPU *spu);
int readCode (SPU *spu, FILE *asmFile);
int makeCode (SPU *spu);
int runCode (SPU *spu);

#endif 