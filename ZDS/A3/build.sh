#!/bin/bash
#gcc main.c -o main -Wall -Wextra -g -O2 -fsanitize=address
gcc -g -fsanitize=address main.c -o main
