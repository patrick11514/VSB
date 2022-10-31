#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char p[28];
    char *check;
    check = fgets(p, 28, stdin);
    if (check == NULL)
    {
        printf("Nepovedlo se načíst text.");
        return -1;
    }

    // split in " "
    int param = 0;
    int len = strlen(p);
    for (int i = 0; i < len; i++)
    {
        if (p[i] >= '0' && p[i] <= '9')
        {
            param = (param * 10) + (p[i] - '0');
        }
    }

    // 20 dig. number
    char n[21];
    check = fgets(n, 21, stdin);
    if (check == NULL)
    {
        printf("Nepovedlo se načíst text.");
        return -1;
    }

    for (int i = 0; i < atoi(n); i++)
    {
        char word[50];
        check = fgets(word, 50, stdin);
        if (check == NULL)
        {
            printf("Nepovedlo se načíst text.");
            return -1;
        }

        int len = strlen(word) - 1;

        if (param <= len)
        {
            int l = 0;
            int r = 0;

            // assign ofsets from sides
            for (int i = 0; i < param; i++)
            {
                if (i % 2 == 0)
                    l++;
                else
                    r++;
            }

            for (int i = param; i < len; i++)
            {
                if (i % 2 == 0)
                {
                    printf("%c", word[l]);
                    l++;
                }
                else
                {
                    printf("%c", word[len - r - 1]);
                    r++;
                }
            }
        }

        printf("\n");
    }

    return 0;
}
