#include <strings.h>

// ======================= [ GLOBAL FUNCTIONS ] ========================

char *splitInputAt(char *input)
{
    return strtok(input, ";\n");
}

// =====================================================================