#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void draw(int *arr, int rows, int cols)
{
    for (int i = 0; i < rows * cols; i++)
    {
        if (i % cols == 0 && i != 0)
        {
            printf("\n");
        }

        if (arr[i] == 0)
        {
            printf(".");
        }
        // BONUS 1
        else if (arr[i] == 2)
        {
            printf("#");
        }
        //===
        else
        {
            printf("o");
        }
    }

    printf("\n");
}

void move(char direction, int *x, int *y, int rows, int cols)
{
    switch (direction)
    {
    case '<':
    {
        if (*x == 0)
        {
            *x = cols - 1;
        }
        else
        {
            (*x)--;
        }
        break;
    }
    case '>':
    {
        if (*x == cols - 1)
        {
            *x = 0;
        }
        else
        {
            (*x)++;
        }
        break;
    }
    case '^':
    {
        if (*y == 0)
        {
            *y = rows - 1;
        }
        else
        {
            (*y)--;
        }
        break;
    }
    case 'v':
    {
        if (*y == rows - 1)
        {
            *y = 0;
        }
        else
        {
            (*y)++;
        }
        break;
    }
    }
}

int putO(int *arr, int x, int y, int cols)
{
    arr[cols * y + x] = 1;
}

// BONUS 1

int generateRandomNumber(int max)
{
    return rand() % (max + 1);
}

int generateRandomObstacles(int *arr, int rows, int cols)
{
    srand((int)time(NULL));

    int obstacles = generateRandomNumber(rows * cols - 1);

    for (int i = 0; i < obstacles; i++)
    {
        int number = generateRandomNumber(rows * cols - 1);
        arr[number] = 2;
    }
}

void moveBonus(char direction, int *arr, int *x, int *y, int rows, int cols)
{
    switch (direction)
    {
    case '<':
    {
        if (*x == 0)
        {
            *x = cols - 1;
        }
        else
        {
            (*x)--;
        }

        if (arr[cols * (*y) + (*x)] == 2)
        {
            moveBonus('>', arr, x, y, rows, cols);
        }
        break;
    }
    case '>':
    {
        if (*x == cols - 1)
        {
            *x = 0;
        }
        else
        {
            (*x)++;
        }
        if (arr[cols * (*y) + (*x)] == 2)
        {
            moveBonus('<', arr, x, y, rows, cols);
        }
        break;
    }
    case '^':
    {
        if (*y == 0)
        {
            *y = rows - 1;
        }
        else
        {
            (*y)--;
        }
        if (arr[cols * (*y) + (*x)] == 2)
        {
            moveBonus('v', arr, x, y, rows, cols);
        }
        break;
    }
    case 'v':
    {
        if (*y == rows - 1)
        {
            *y = 0;
        }
        else
        {
            (*y)++;
        }
        if (arr[cols * (*y) + (*x)] == 2)
        {
            moveBonus('^', arr, x, y, rows, cols);
        }
        break;
    }
    }
}

// BONUS 2
void clearScreen()
{
    printf("\x1b[2J\n");
}

void drawTurtle(int *arr, int rows, int cols, int x, int y)
{
    for (int i = 0; i < rows * cols; i++)
    {
        if (i % cols == 0 && i != 0)
        {
            printf("\n");
        }

        if (cols * y + x == i)
        {
            printf("Z");
        }
        else if (arr[i] == 0)
        {
            printf(".");
        }
        // BONUS 1
        else if (arr[i] == 2)
        {
            printf("#");
        }
        //===
        else
        {
            printf("o");
        }
    }

    printf("\n");
}
// ======

int main()
{
    const int obstacles = true;
    const int animate = false;

    int rows, cols;

    scanf("%d %d", &rows, &cols);

    int *arr = (int *)calloc(rows * cols, sizeof(int));

    int x = 0, y = 0;

    if (obstacles)
        generateRandomObstacles(arr, rows, cols);

    int end = false;

    while (!end)
    {
        char input;
        scanf("%c", &input);

        switch (input)
        {
        case 'x':
        {
            if (animate)
                clearScreen();

            draw(arr, rows, cols);
            end = true;
            break;
        }
        case '<':
        case '>':
        case '^':
        case 'v':
        {
            if (!obstacles)
                move(input, &x, &y, rows, cols);
            // BONUS 1
            else
                moveBonus(input, arr, &x, &y, rows, cols);

            if (animate)
            {
                clearScreen();
                drawTurtle(arr, rows, cols, x, y);
                usleep(500 * 1000);
            }
            break;
        }
        case 'o':
        {
            putO(arr, x, y, cols);
            break;
        }
        }
    }

    free(arr);
    return 0;
}
