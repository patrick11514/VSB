// h files
#include "dictionary.h"
#include "vector.h"
// libraries
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bool dictionaryInit(Dictionary *dict)
{
    dict->en = vectorInit(_VECTOR_DEFAULT_CAPACITY);
    dict->cz = vectorInit(_VECTOR_DEFAULT_CAPACITY);
    if (dict->en == NULL || dict->cz == NULL)
    {
        return false;
    }
    dict->size = 0;
    return true;
}

bool dictionaryFree(Dictionary *dict)
{
    // free ptrs with strings
    for (int i = 0; i < dict->size; i++)
    {
        free(dict->en->data[i]);
        free(dict->cz->data[i]);
    }

    if (!vectorFree(dict->en, true))
        return false;
    if (!vectorFree(dict->cz, true))
        return false;
    free(dict);
    dict = NULL;
    return true;
}

bool dictionaryAdd(Dictionary *dict, char *en, char *cz)
{
    if (!vectorAdd(dict->en, en))
        return false;

    if (!vectorAdd(dict->cz, cz))
        return false;

    dict->size++;

    return true;
}

void *dictionaryGet(Dictionary *dict, char *string, bool inverse)
{
    for (int i = 0; i < dict->size; i++)
    {
        if (!inverse)
        {
            if (strcmp(dict->en->data[i], string) == 0)
            {
                return dict->cz->data[i];
            }
        }
        else
        {
            if (strcmp(dict->cz->data[i], string) == 0)
            {
                return dict->en->data[i];
            }
        }
    }

    return NULL;
}

void dictionaryLoad(FILE *f, Dictionary *dict)
{
    char line[100];

    while (fgets(line, 100, f) != NULL)
    {
        char *en = strdup(strtok(line, ","));
        char *cz = strdup(strtok(NULL, "\n"));

        if (!dictionaryAdd(dict, en, cz))
        {
            printf("Could not load dictionary\n");
            exit(1);
        }
    }
}