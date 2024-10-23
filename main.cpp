#include "stack.h"
#include "operations.h"
#include "stackUserInterface.h"


int main ()
{ 
    SPU spu = {};
    stackCtor (&spu.stack, 10);

    if (readFile (&spu) == 1)
        return 1;

    spu.regs[1] = 7;
    spu.RAM[78] = 1;
    spu.RAM[9] = 3;

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