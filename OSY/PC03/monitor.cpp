#include <cstring>
#include <random>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("Enter file to watch\n");
        return EXIT_FAILURE;
    }

    struct stat oldStatus;
    bool init = false;

    while (true)
    {
        // 500ms
        usleep(500 * 1000);

        struct stat newStatus;
        if (stat(argv[1], &newStatus) != 0)
        {
            printf("Unable to get status of file\n");
            return EXIT_FAILURE;
        }

        if (!init)
        {
            oldStatus = newStatus;

            init = true;
            continue;
        }

        if (oldStatus.st_mtim.tv_nsec == newStatus.st_mtim.tv_nsec)
        {
            continue;
        }

        if (oldStatus.st_size > newStatus.st_size)
        {
            printf("File was truncated\n");
        }
        else
        {
            FILE *file = fopen(argv[1], "r");

            fseek(file, oldStatus.st_size, SEEK_SET);

            char buffer[1000];

            while (true)
            {
                size_t readed = fread(buffer, sizeof(char), 1000, file);

                if (readed == 0)
                {
                    break;
                }

                fwrite(buffer, sizeof(char), readed, stdout);
            }
        }

        oldStatus = newStatus;
    }

    return EXIT_SUCCESS;
}