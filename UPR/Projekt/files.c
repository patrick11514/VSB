// basic includes
#include <stdio.h>
#include <string.h>
#include <errno.h>

// my includes
#include "files.h"

// =============== [ FUNCTIONS WORKING WITH FILES ] ====================

FILE *openFile(char *filename, char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        printf("Nepovedlo se otevřít soubor. (%d - %s)\n", errno, strerror(errno));
    }
    return file;
}
// =====================================================================