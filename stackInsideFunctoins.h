#ifndef STACK_H_
#define STACK_H_

#include "stackError.h"
#include "stack.h"

#define NUMBER_FOR_CALCULATION_HASH 2

StackErrors stackExtend (Stack *stack, int line, const char* function, const char* file);
StackErrors stackReduce (Stack *stack, int line, const char* function, const char* file);
int dataHash (int *array, int arraySize);

#endif