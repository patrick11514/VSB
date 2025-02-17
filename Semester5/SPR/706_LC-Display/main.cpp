#include <cstdio>
#include <cstdlib>
#include <cstring>

/*
Segments:
 --    A
|  |  B C
 --    D
|  |  E F
 --    G

aka

[A, B, C, D, E, F, G]
*/

static const bool numbers[10][7] = {
    {true, true, true, false, true, true, true},     // 0
    {false, false, true, false, false, true, false}, // 1
    {true, false, true, true, true, false, true},    // 2
    {true, false, true, true, false, true, true},    // 3
    {false, true, true, true, false, true, false},   // 4
    {true, true, false, true, false, true, true},    // 5
    {true, true, false, true, true, true, true},     // 6
    {true, false, true, false, false, true, false},  // 7
    {true, true, true, true, true, true, true},      // 8
    {true, true, true, true, false, true, true},     // 9
};

static const bool *getShape(char c) { return numbers[c - '0']; }

static char row[99999999];

void printNumbers(int size, int len) {
  int idx = 0;
  // ABC,DEF,G
  for (int repeat = 0; repeat < 3; ++repeat) {
    int shapeRowPart = repeat * 3; // item 0, 3, 6

    for (int i = 0; i < len; ++i) {
      const bool *shape = getShape(row[i]);

      putchar(' ');
      char print = ' ';
      if (shape[shapeRowPart]) {
        print = '-';
      }

      for (int n = 0; n < size; ++n)
        putchar(print);

      putchar(' ');
      if (len - 1 != i)
        putchar(' ');
    }

    putchar('\n');

    if (repeat == 2)
      return;

    for (int n = 0; n < size; ++n) {
      for (int i = 0; i < len; ++i) {
        const bool *shape = getShape(row[i]);

        putchar(shape[shapeRowPart + 1] ? '|' : ' ');

        for (int c = 0; c < size; ++c)
          putchar(' ');

        putchar(shape[shapeRowPart + 2] ? '|' : ' ');
        if (len - 1 != i)
          putchar(' ');
      }

      putchar('\n');
    }
  }
}

int main() {
  while (true) {
    int size;
    scanf("%d ", &size);

    if (size == 0)
      break;

    int i = 0;

    while (true) {
      char c = getchar();
      if (c == '\n') {
        break;
      }

      row[i++] = c;
    }

    printNumbers(size, i);
    putchar('\n');
  }
  return 0;
}
