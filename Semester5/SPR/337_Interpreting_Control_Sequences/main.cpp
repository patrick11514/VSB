#include <cstdio>
#include <cstdlib>
#define DEBUG 0

// Return values
#define CONTINUE 1
#define END 0
// Modes
#define INSERT 1
#define OVERWRITE 2

static char display[10][10] = {};
static int x = 0;
static int y = 0;
static int mode = OVERWRITE;
static int lines = 0;
static int elapsed = 0;

void initDisplay() {
  for (int y = 0; y < 10; ++y)
    for (int x = 0; x < 10; ++x)
      display[y][x] = ' ';
}

int writeChar(char c) {
  if (mode == OVERWRITE) {
    display[y][x] = c;
  } else {

    // insert
    for (int col = 8; col >= x; --col) {
      display[y][col + 1] = display[y][col];
    }
    display[y][x] = c;
  }

  if (x < 9)
    x++;

  return CONTINUE;
}

int readInstruction() {
  char first = getchar();
  if (first == '\n') {
    ++elapsed;
    return elapsed < lines ? CONTINUE : END; // end of end
  }

  if (first != '^') {
    return writeChar(first);
  }

  char second = getchar();
  // interpret instructions
  switch (second) {
  case 'b':
    x = 0;
    break;
  case 'c':
    initDisplay();
    break;
  case 'd':
    if (y < 9)
      y++;
    break;
  case 'e':
    for (int col = x; col <= 9; ++col)
      display[y][col] = ' ';
    break;
  case 'h':
    x = y = 0;
    break;
  case 'i':
    mode = INSERT;
    break;
  case 'l':
    if (x > 0)
      x--;
    break;
  case 'o':
    mode = OVERWRITE;
    break;
  case 'r':
    if (x < 9)
      x++;
    break;
  case 'u':
    if (y > 0)
      y--;
    break;
  case '^':
    writeChar('^');
    break;
  default:
    // move cursor sequence
    y = second - '0';
    char third = getchar();
    x = third - '0';
    break;
  }

  return CONTINUE;
}

void printScreen() {
  printf("+----------+\n");
  for (int row = 0; row < 10; ++row) {
    putchar('|');
    for (int col = 0; col < 10; ++col) {
#if DEBUG == 1
      if (x == col && y == row) {
        printf("\e[0;34m");
        char c = display[row][col];
        if (c == ' ')
          putchar('@');
        else
          putchar(c);
        printf("\e[0m");
      } else
#endif
      {

        putchar(display[row][col]);
      }
    }
    putchar('|');
    putchar('\n');
  }
  printf("+----------+\n");
}

int main() {
  int n = 0;

  while (true) {
    ++n;

    elapsed = 0;
    scanf("%d\n", &lines);

    if (lines == 0)
      break;

    // reset
    mode = OVERWRITE;
    x = y = 0;
    initDisplay();

    while (true) {
      if (readInstruction() == END)
        break;
#if DEBUG == 1
      printScreen();
#endif
    }

    // print display and loop
    printf("Case %d\n", n);
    printScreen();
  }
  return 0;
}
