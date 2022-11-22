// basic includes
#include <stdio.h>
#include <string.h>
#include <errno.h>

// header files
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

int getLines(FILE *fp)
{
    char ch;
    int lines = 0;

    while (!feof(fp))
    {
        ch = fgetc(fp);
        if (ch == '\n')
        {
            lines++;
        }
    }

    return lines;
}

// =====================================================================