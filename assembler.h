#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "spu.h"
#include "sign.h"
#include "runCodeOperations.h"

const int MAX_COMMAND_SIZE = 10;

ERROR_CODES readFile (SPU *spu);
CMD makeBinFile (FILE *asmFile, FILE *binFile, Labels *labels, int *ip);
int seekLabels (FILE *asmFile, FILE *binFile, Labels *labels, int *ip);
int pushRamArgAndReg (FILE *binFile, char *str, int *ip, char regLetter, int regNumber);
int pushRamReg (FILE *binFile, char *str, int *ip, char regLetter, int regNumber);
int pushReg (FILE *binFile, char *str, int *ip, char regLetter, int regNumber);
int jumps (FILE *asmFile, FILE *binFile, char *cmd, int *ip, Labels *labels, const char *jumpType, int jumpNumber);

#endif