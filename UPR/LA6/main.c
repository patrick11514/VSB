#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

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

    t1[0] = 'a';
    t2[0] = 'a';
    // t3[0] = 'a';
    // t4[0] = 'a';

    printf("1: %s\n", t1);
    printf("2: %s\n", t2);
    printf("3: %s\n", t3);
    printf("4: %s\n", t4);*/

    const char *dlouhyText = "gmsd sdk mfsd mk sdf skd ds fl sdfl\
    \
    fsdkg sd fsd fsd ";

    const char *dlouhyText2 = "gmsd sdk mfsd mk sdf skd ds fl sdfl"
                              ""
                              "fsdkg sd fsd fsd";

    return 0;
}
