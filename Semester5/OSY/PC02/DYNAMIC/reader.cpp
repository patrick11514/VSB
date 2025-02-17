#include "reader.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

using std::size_t;

void input()
{
    char buffer[1024];
    size_t len = 0;
    int line = 1;

    while (fgets(buffer, 1024, stdin))
    {
        int index = 0;
        int len = strlen(buffer);
        --len; // remove \n

        int sum = 0;

        while (index < len)
        {
            int num, offset;
            sscanf(buffer + index, "%d%n", &num, &offset);

            index += offset;

            if (index == len)
            {
                if (sum != num)
                {
                    printf("Line %d sum is invalid, valid sum should be %d\n", line, sum);
                }
            }
            else
            {
                sum += num;
            }
        }

        ++line;
    }
}
