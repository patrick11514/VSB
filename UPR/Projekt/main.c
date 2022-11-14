#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

FILE *openFile(char *filename, char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        printf("Nepovedlo se otevřít soubor. (%d - %s)\n", errno, strerror(errno));
    }
    return file;
}

typedef struct
{
    int count;
    int *scores;
    char **players;
} Highscores;

char *readInput(char *input)
{
    return strtok(input, ";\n");
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

Highscores *readHighscores(FILE *file)
{
    char row[100];

    int count = 0;

    int lines = getLines(file);

    // tries to allocate players and scores by

    int *scores = (int *)malloc(sizeof(int) * lines);
    char **players = (char **)malloc(sizeof(char *) * lines);

    if (!scores || !players)
    {
        printf("Unable to allocate memory");
        exit(1);
    }

    fseek(file, 0, 0);

    while (fgets(row, 100, file) != NULL)
    {
        char *name = readInput(row);

        char *scoreStr = readInput(NULL);
        if (!scoreStr)
            continue;

        char *saveName = (char *)malloc(sizeof(char) * strlen(name) + 1);
        if (!strcpy(saveName, name))
        {
            printf("Unable to copy string");
            exit(1);
        }
        int score = atoi(scoreStr);

        players[count] = saveName;
        scores[count] = score;

        count++;
    }

    Highscores *highscores = (Highscores *)malloc(sizeof(Highscores));

    if (!highscores)
    {
        printf("Unable to allocate memory");
        exit(1);
    }

    int *realScores = (int *)malloc(sizeof(int) * count);
    char **realPlayers = (char **)malloc(sizeof(char *) * count);

    for (int i = 0; i < count; i++)
    {
        realScores[i] = scores[i];
        realPlayers[i] = players[i];
    }

    free(players);
    free(scores);

    highscores->count = count;
    highscores->players = realPlayers;
    highscores->scores = realScores;

    return highscores;
}

void addHighscore(Highscores *highscores, char *name, int score)
{
    char **players = (char **)malloc(sizeof(char *) * (highscores->count + 1));
    int *scores = (int *)malloc(sizeof(int) * (highscores->count + 1));

    for (int i = 0; i < highscores->count; i++)
    {
        players[i] = highscores->players[i];
        scores[i] = highscores->scores[i];
    }

    char *newName = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (!strcpy(newName, name))
    {
        printf("Unable to copy string");
        exit(1);
    }

    players[highscores->count] = newName;
    scores[highscores->count] = score;

    free(highscores->players);
    free(highscores->scores);

    highscores->players = players;
    highscores->scores = scores;
    highscores->count = highscores->count + 1;
}

void swapScores(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void swapNames(char **x, char **y)
{
    char *temp = *x;
    *x = *y;
    *y = temp;
}

// sort from highest to lowest
void sortHighscores(Highscores *highscores)
{
    int *scores = highscores->scores;
    char **names = highscores->players;
    int count = highscores->count;

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                swapScores(scores + j, scores + j + 1);
                swapNames(names + j, names + j + 1);
            }
        }
    }
}

void writeHighscores(Highscores *highscores)
{
    FILE *file = openFile("highscores.txt", "wt");

    for (int i = 0; i < highscores->count; i++)
    {
        fprintf(file, "%s;%d\n", highscores->players[i], highscores->scores[i]);
    }

    fclose(file);

    for (int i = 0; i < highscores->count; i++)
    {
        free(highscores->players[i]);
    }

    free(highscores->players);
    free(highscores->scores);
    free(highscores);
}

int main()
{
    FILE *file = openFile("highscores.txt", "rt");

    Highscores *highscores;

    if (!file)
    {
        highscores = (Highscores *)malloc(sizeof(Highscores));
        highscores->count = 0;
        highscores->players = NULL;
        highscores->scores = NULL;
    }
    else
    {
        highscores = readHighscores(file);
        fclose(file);
    }

    char row[100];

    printf("Enter new highscore (username;score):\n");

    if (!fgets(row, 100, stdin))
    {
        printf("Can't read input");
        exit(1);
    }

    char *username = readInput(row);
    char *scoreStr = readInput(NULL);

    if (!scoreStr)
    {
        printf("No score found.");
        exit(1);
    }

    int score = atoi(scoreStr);

    addHighscore(highscores, username, score);

    sortHighscores(highscores);

    writeHighscores(highscores);

    return 0;
}
