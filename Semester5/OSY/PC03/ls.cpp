#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

struct Arg
{
    int order = 0;
    int position = 0;
};

int main(int argc, char **argv)
{
    int order = 1;
    struct Arg size;
    struct Arg time;
    struct Arg perms;

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-s") == 0)
        {
            size.order = order;
            size.position = i;
            ++order;
            continue;
        }
        else if (strcmp(argv[i], "-t") == 0)
        {
            time.order = order;
            time.position = i;
            ++order;
            continue;
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            perms.order = order;
            perms.position = i;
            ++order;
            continue;
        }
    }

    int index = 0;
    int invalid[argc];

    printf("\n");

    bool attr = false;

    if (0 != size.order || 0 != time.order || 0 != perms.order)
    {
        attr = true;
    }

    if (attr)
    {
        for (int i = 1; i < order; ++i)
        {
            if (size.order == i)
            {
                printf("%12s ", "VELKOST");
            }
            else if (time.order == i)
            {
                printf("%24s ", "VYTVOŘENO");
            }
            else if (perms.order == i)
            {
                printf("%12s ", "OPRÁVNĚNÍ");
            }
        }

        printf("%12s\n", "NÁZEV");
    }

    for (int i = 1; i < argc; ++i)
    {
        // skip args with parameters
        if (i == size.position || i == time.position || i == perms.position)
            continue;

        struct stat data;

        if (stat(argv[i], &data) != 0)
        {
            invalid[index] = i;
            ++index;
            continue;
        }

        for (int param = 1; param < order; ++param)
        {
            if (size.order == param)
            {
                printf("%12ld ", data.st_size);
            }
            else if (time.order == param)
            {
                char *time = ctime(&data.st_ctim.tv_sec);
                // remove new line
                time[strlen(time) - 1] = ' ';
                printf("%24s ", time);
            }
            else if (perms.order == param)
            {
                for (int perm = R_OK; perm >= X_OK; perm /= 2)
                {
                    if (access(argv[i], perm) == 0)
                    {
                        switch (perm)
                        {
                        case R_OK:
                            printf("r");
                            break;
                        case W_OK:
                            printf("w");
                            break;
                        case X_OK:
                            printf("x");
                            break;
                        }
                    }
                    else
                    {
                        if (errno == EACCES)
                        {
                            printf("-");
                        }
                        else
                        {
                            printf("?");
                        }
                    }
                }

                printf("%9s ", "");
            }
        }

        printf("%s", argv[i]);

        if (attr)
        {
            printf("\n");
        }
        else
        {
            printf(" ");
        }
    }

    printf("\n");

    if (index > 0)
    {
        printf("\n---------------------------\n\nThese files were not found:\n");
        for (int i = 0; i < index; ++i)
        {
            printf("- %s\n", argv[invalid[i]]);
        }
    }

    return 0;
}
