#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

FILE *openFile(char *filename, char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        printf("Nepovedlo se otevřít soubor. (%d - %s)\n", errno, strerror(errno));
        exit(1);
    }
    return file;
}

typedef struct
{
    int count;
    int *scores;
    char *players;
} Highscores;

char *readInput(char *input)
{
    return strtok(input, " \n");
}

Highscores *readHighscores(FILE *file)
{
    Highscores *highscores = (Highscores *)malloc(sizeof(Highscores));

    char *row[100];

    int count = 0;

    while (fgets(row, 100, file) != NULL)
    {
        char *name = readInput(row);

        char *score = readInput(NULL);
        if (!score)
            continue;

        char *saveName = (char *)malloc(sizeof(char) * strlen(name));
        strcpy(saveName, name);
        int score = atoi(score);

        count++;
    }
}

int main(int argc, char **argv)
{
    FILE *file = openFile("highscores.txt", "rt");

    return 0;
}
