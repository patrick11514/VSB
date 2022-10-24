#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    /*char t1[] = "Ahoj"; // ekvivalent k řádku t2
    char t2[] = {'A', 'h', 'o', 'j', '\0'};
    const char *t3 = "Ahoj";
    char *t4 = "Ahoj";

    printf("1: %s\n", t1);
    printf("2: %s\n", t2);
    printf("3: %s\n", t3);
    printf("4: %s\n", t4);

    printf("1: %c\n", t1[0]);
    printf("2: %c\n", t2[0]);
    printf("3: %c\n", t3[0]);
    printf("4: %c\n", t4[0]);

    // t1[0] = 'a';
    // t2[0] = 'a';
    // t3[0] = 'a';
    // t4[0] = 'a';

    // printf("1: %s\n", t1);
    // printf("2: %s\n", t2);
    // printf("3: %s\n", t3);
    // printf("4: %s\n", t4);

    // printf("Shoda: %d %d %d %d %d\n", t1 == t1, t1 == t2, t1 == t3, t1 == t4, t1 == "Cau");
    // printf("Shoda strcmp: %d %d %d %d\n", strcmp(t1, t1), strcmp(t1, t2), strcmp(t1, t3), strcmp(t1, t4));

    char a = 'a';
    // na velke
    printf("%c\n", a - 'a' + 'A');

    printf("%d\n", atoi("456"));*/

    // nacte pouze 79 znaku, protože funkce fgets načítá do n-1
    char retezec[80];
    fgets(retezec, sizeof(retezec), stdin);

    printf("Nacteno %d znaku: \n%s\n", strlen(retezec), retezec);

    return 0;
}
