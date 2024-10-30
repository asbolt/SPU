#ifndef SIGN_H_
#define SIGN_H_

#include "spu.h"

int writeSignature (FILE *binFile, FILE *asmFile);
int checkSignature (FILE *binFile);

#endif