#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "dictionary.h"

int main(int argc, char **argv)
{
    // check parameters
    if (argc < 3)
    {
        printf("Wrong parameters\n");
        return 1;
    }

    bool reverseTranslate = false;

    // if 4th argument is "reverse" set reverse translate to true
    if (argc > 4)
    {
        if (strcmp(argv[4], "reverse") == 0)
        {
            reverseTranslate = true;
        }
    }

    // open dictionary file
    FILE *dictFile = fopen(argv[1], "rt");

    if (!dictFile)
    {
        printf("Could not load dictionary\n");
        exit(1);
    }

    // open input file
    FILE *input = fopen(argv[2], "rt");
    if (!input)
    {
        printf("Could not open input file\n");
        return 1;
    }

    // open output file
    FILE *output = fopen(argv[3], "wt");
    if (!output)
    {
        printf("Could not open output file\n");
        return 1;
    }

    // initialize dictionary
    Dictionary *dictionary = (Dictionary *)malloc(sizeof(Dictionary));
    if (!dictionaryInit(dictionary))
    {
        printf("Could not load dictionary\n");
        exit(1);
    }

    // load dictionary
    dictionaryLoad(dictFile, dictionary);

    fclose(dictFile);

    // 300 because of reverse translation some lines are longer than 255 characters
    char line[300];

    // while until end of file
    while (fgets(line, 300, input) != NULL)
    {
        char *word = strtok(line, " \n");

        int c = 0;
        // while until end of line
        while (word != NULL)
        {
            char *translated = (char *)dictionaryGet(dictionary, word, reverseTranslate);
            // before every word than first add space
            if (c > 0)
            {
                fprintf(output, " ");
            }

            if (translated == NULL)
            {
                fprintf(output, "<unknown>");
            }
            else
            {
                fprintf(output, "%s", translated);
            }

            word = strtok(NULL, " \n");
            c++;
        }

        fprintf(output, "\n");
    }

    fclose(input);
    fclose(output);

    if (!dictionaryFree(dictionary))
    {
        printf("Could not free dictionary");
        return 1;
    }

    return 0;
}
