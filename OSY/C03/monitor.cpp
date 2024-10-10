#include <cstring>
#include <random>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

struct MonitorFile
{
    bool init = false;
    bool ignored = false;
    struct stat status;
};

int main(int argc, char **argv)
{
    if (argc <= 3 || strcmp(argv[1], "-t") != 0)
    {
        printf("Usage %s -t msec <file1> [<file2>]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int monitorInterval = std::stoi(argv[2]);

    struct MonitorFile statuses[argc - 3];

    for (int i = 2; i < argc; ++i)
    {
        if (access(argv[i], X_OK) == 0 || (access(argv[i], W_OK) != 0 && errno == EACCES))
        {
            statuses[i - 3].ignored = true;
            printf("File %s is ignored\n", argv[i]);
        }
    }

    while (true)
    {
        for (int i = 3; i < argc; ++i)
        {
            int arrIndex = i - 3;

            if (statuses[arrIndex].ignored)
                continue;

            struct stat status;

            if (stat(argv[i], &status) != 0)
            {
                printf("Unable to get status of file %s\n", argv[i]);
                continue;
            }
            if (!statuses[arrIndex].init)
            {
                statuses[arrIndex].status = status;
                statuses[arrIndex].init = true;
                continue;
            }

            struct stat prevStatus = statuses[arrIndex].status;

            statuses[arrIndex].status = status;

            if (status.st_mtim.tv_nsec == prevStatus.st_mtim.tv_nsec)
            {
                continue;
            }

            char *time = ctime(&status.st_ctim.tv_sec);
            time[strlen(time) - 1] = ' ';

            printf("   -----------------------------------------------------------------\n");
            printf("   --- %s %8ld %s ----------------------------\n", time, status.st_size, argv[i]);

            if (status.st_size < prevStatus.st_size)
            {
                printf("> File was truncated\n");
            }
            else
            {
                FILE *file = fopen(argv[i], "r");

                fseek(file, prevStatus.st_size, SEEK_SET);

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

            printf("\n");
        }

        // Xms
        usleep(1000 * monitorInterval);
    }

    return EXIT_SUCCESS;
}