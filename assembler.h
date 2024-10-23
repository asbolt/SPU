#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "spu.h"
#include "sign.h"
#include "runCodeOperations.h"

int readFile (SPU *spu);
CMD assembler (FILE *ptrFile, FILE *asmFile, Labels *labels, int *ip);
int firstComp (FILE *ptrFile, FILE *asmFile, Labels *labels, int *ip);
int pushRamArgAndReg (FILE *asmFile, char *str, int *ip, char regLetter, int regNumber);
int pushRamReg (FILE *asmFile, char *str, int *ip, char regLetter, int regNumber);
int pushReg (FILE *asmFile, char *str, int *ip, char regLetter, int regNumber);
int jumps (FILE *ptrFile, FILE *asmFile, char *cmd, int *ip, Labels *labels, const char *jumpType, int jumpNumber);

#endif