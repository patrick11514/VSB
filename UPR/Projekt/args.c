// basic include
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// my include
#include "args.h"

bool loadArgs(int argc, char **argv, bool *levels, bool *includeDefaultLevels)
{
    if (argc > 1)
    {
        // check if arguments contains --help
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "--help") == 0)
            {
                printf("Usage: %s --[OPTION]=[VALUE]...\n", argv[0]);
                printf("Options:\n");
                printf("help - show this message\n");
                printf("level - load level from given file\n");
                printf("      - Example: %s --level=level1.txt\n", argv[0]);
                printf("      - You could also load more levels separated by semicolon\n");
                printf("      - Example: %s --level=level1.txt,level2.yml\n", argv[0]);
                printf("      - Otherwise it will load default levels\n");
                printf("includeDefaultLevels - include default levels\n");
                printf("      - Example: %s --includeDefaultLevels=false\n", argv[0]);
                printf("      - Otherwise it will only include levels from levels argument.\n");

                return true;
            }
            // check if arguments contains --level
            else if (strncmp(argv[i], "--level=", 8) == 0)
            {
                // get level file
                char *levelFiles = argv[i] + 8;
                printf("%s\n", levelFiles);

                *levels = true;

                return true;
            }

            // load default levels beside given levels
            else if (strncmp(argv[i], "--includeDefaultLevels=", 23) == 0)
            {
                // get level file
                char *includeDL = argv[i] + 23;
                if (strcmp(includeDL, "false") != 0 && strcmp(includeDL, "true") != 0)
                {
                    printf("Unknown value for argument: %s, skipping\n", argv[i]);
                }
                else
                {
                    printf("%s\n", includeDL);

                    if (strcmp(includeDL, "true") == 0)
                    {
                        *includeDefaultLevels = true;
                    }
                    else
                    {
                        *includeDefaultLevels = false;
                    }
                }
            }

            // otherwise just show warning
            else
            {
                printf("Unknown argument: %s, skipping\n", argv[i]);
            }
        }
    }

    return false;
}
