#ifndef ERROR_H_
#define ERROR_H_

#include "stack.h"

#define STACK_CHECK(stack, line, function, file) do { \
    if (stackCheckForError (stack) != STACK_GOOD)\
        {\
            stackPrintError (stack, line, function, file);\
            return stack->stackError;\
        }\
  } while(0)

StackErrors stackCheckForError (Stack *stack);
StackErrors stackPrintError (Stack *stack, int line, const char* function, const char* file);

#endif