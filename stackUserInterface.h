#ifndef ELEMENTS_H_
#define ELEMENTS_H_

#include "stackInsideFunctoins.h"
#include "stack.h"

StackErrors stackPush (Stack *stack, stackElementType element, int line, const char* function, const char* file);
int stackPop (Stack *stack, int line, const char* function, const char* file);
StackErrors stackCtor (Stack *stack, stackElementType size);
StackErrors stackDtor (Stack *stack, int line, const char* function, const char* file);
StackErrors stackDump (Stack *stack);

#endif
