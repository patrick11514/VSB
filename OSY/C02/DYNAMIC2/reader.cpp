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
        char lastSymbol = '\0';

        while (index < len)
        {
            int num, offset;
            char symbol;
            int readed = sscanf(buffer + index, "0x%X%c%n", &num, &symbol, &offset);

            if (readed != 2)
            {
                printf("Line %d has invalid format\n", line);
                break;
            }

            index += offset;

            if (lastSymbol != '\0')
            {
                switch (lastSymbol)
                {
                case '*':
                    sum *= num;
                    break;
                case '/':
                    sum /= num;
                    break;
                case '%':
                    sum %= num;
                    break;
                }
            }
            else
            {
                sum = num;
            }

            lastSymbol = symbol;

            if (index == len)
            {
                printf("%d\n", sum);
            }
        }

        ++line;
    }
}
