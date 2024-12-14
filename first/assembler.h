#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "spu.h"
#include "sign.h"
//#include "runCodeOperations.h"

const int MAX_COMMAND_SIZE = 10;
const int RAM_NUMBER = 5;
const int RAM_WITH_PLUS = 8;

ERROR_CODES readFile (SPU *spu);
int compilation (FILE *asmFile, FILE *binFile, Labels *labels, int *ip);
REGS determineRegister (char *command);
int readImmediate (char *command);
int checkRAM (char *command);
int determineLabel (char *command, Labels *labels, FILE *binFile, int *ip);
CMD readCommand (char *command, int *ip, FILE *binFile, Labels *labels);

#endif