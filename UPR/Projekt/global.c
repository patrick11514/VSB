// basic includes
#include <string.h>

// header files
#include "global.h"
#include "breakout.h"

// ======================= [ GLOBAL FUNCTIONS ] ========================

char *splitInput(char *input)
{
    return strtok(input, ";\n");
}

float getScale(int displayHeight)
{
    if (WINDOW_HEIGHT > displayHeight)
    {
        if ((WINDOW_HEIGHT * 0.75) > displayHeight)
        {
            return 0.5;
        }
        else
        {
            return 0.75;
        }
    }
    return 1.0;
}

// =====================================================================