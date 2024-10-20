#include "stack.h"
#include "operations.h"
#include "stackUserInterface.h"


int main ()
{ 
    SPU spu = {};
    stackCtor (&spu.stack, 10);

    readFile (&spu);

    while (1)
    {
        if (operations(&spu) < 0)
        {
            stackDtor (&spu.stack, VALUES_FOR_ERROR);
            return 0;
        }
    }

    return 0;
}