// basic includes
#include <string.h>

// header files
#include "global.h"

// ======================= [ GLOBAL FUNCTIONS ] ========================

char *splitInput(char *input)
{
    return strtok(input, ";\n");
}

// =====================================================================