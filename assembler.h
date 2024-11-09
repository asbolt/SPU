#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "spu.h"
#include "sign.h"
#include "runCodeOperations.h"

const int MAX_COMMAND_SIZE = 10;
const int RAM_NUMBER = 5;
const int RAM_WITH_PLUS = 8;

ERROR_CODES readFile (SPU *spu);
int compilation (FILE *asmFile, FILE *binFile, Labels *labels, int *ip);
REGS determiningRegister (char *str);
int readingImmediate (char *str);
int checkRAM (char *str);
int determiningLabel (char *str, Labels *labels, FILE *binFile, int *ip);
CMD readingCommand (char *str, int *ip, FILE *binFile, Labels *labels);

#endif