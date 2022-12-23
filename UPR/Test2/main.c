#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    char *name;
    char *town;
    int naughtiest;
    int presentCount;
} Child;

typedef struct
{
    Child **data;
    int count;
} Children;

Children *loadChildren(int count)
{
    char row[256];

    Children *children = (Children *)malloc(sizeof(Children));
    children->count = count;
    children->data = (Child **)malloc(sizeof(Child *) * count);

    if (children == NULL)
    {
        printf("Unable to allocate memory");
        exit(1);
    }

    if (children->data == NULL)
    {
        printf("Unable to allocate memory");
        exit(1);
    }

    for (int i = 0; i < count; i++)
    {
        if (!fgets(row, 256, stdin))
        {
            printf("Unable to load input");
            exit(1);
        }

        char *name = strtok(row, ",");
        char *town = strtok(NULL, ",");
        char *naughtiest = strtok(NULL, ",");
        char *presentCount = strtok(NULL, "\n");

        if (name == NULL || town == NULL || naughtiest == NULL || presentCount == NULL)
        {
            printf("Unable to load input");
            exit(1);
        }

        Child *current = (Child *)malloc(sizeof(Child));
        if (current == NULL)
        {
            printf("Unable to allocate memory");
            exit(1);
        }

        current->name = strdup(name);
        current->town = strdup(town);
        current->presentCount = atoi(presentCount);
        current->naughtiest = atoi(naughtiest);

        children->data[i] = current;
    }

    return children;
}

void freeChildren(Children *children)
{
    for (int i = 0; i < children->count; i++)
    {
        free(children->data[i]->name);
        free(children->data[i]->town);
        free(children->data[i]);
    }
    free(children->data);
    free(children);
    children = NULL;
}

void loadQueries(int count, Children *children)
{
    char row[255];
    for (int i = 0; i < count; i++)
    {
        if (!fgets(row, 255, stdin))
        {
            printf("Unable to load input");
            exit(1);
        }

        char *queryName = strtok(row, ",");
        char *character = strtok(NULL, "\n");

        if (strcmp(queryName, "naughtiest-named") != 0)
        {
            printf("Invalid query");
            exit(1);
        }

        int highest = 0;
        Child *child = NULL;

        for (int l = 0; l < children->count; l++)
        {
            Child *current = children->data[l];
            if (strchr(current->name, character[0]) != NULL)
            {
                if (current->naughtiest > highest)
                {
                    highest = current->naughtiest;
                    child = current;
                }
            }
        }
        if (child == NULL)
        {
            printf("<not-found>\n");
        }
        else
        {
            printf("%s,%d\n", child->name, child->naughtiest);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Invalid parameters.");
        return 1;
    }

    int count = atoi(argv[1]);
    int queries = atoi(argv[2]);

    Children *children = loadChildren(count);

    loadQueries(queries, children);

    // free
    freeChildren(children);

    return 0;
}
