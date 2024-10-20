#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "stack.h"

enum CMD
{
    PUSH = 1,
    POP = 2,
    ADD = 3,
    SUB = 4,
    MUL = 5,
    DIV = 6,
    OUT = 7,
    HLT = -1,
    ERROR = -2
};

CMD operations (Stack *stack); 

#endif