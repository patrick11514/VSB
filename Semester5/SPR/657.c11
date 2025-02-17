#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <vector>

// forward declaration
int dfs(int x, int y, char c);

enum State { Undiscovered, Discovered, Processed };

struct Pixel {
  char c;
  int x;
  int y;
  State state = State::Undiscovered;
};

static int rows, cols;
static Pixel grid[50][50];

using Stack = std::stack<Pixel *>;

static const int around[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

void print() {
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      Pixel &pixel = grid[row][col];
      if (pixel.state == State::Discovered) {
        printf("\e[0;35m");
      } else if (pixel.state == State::Processed) {
        printf("\e[1;33m");
      }

      putchar(pixel.c);

      if (pixel.state != State::Undiscovered) {
        printf("\e[0m");
      }
    }
    putchar('\n');
  }
}

static bool findContinue = false;
static std::vector<Pixel *> continuePixels;

void getSuroundingPixels(Pixel *pixel, Stack &stack, char c, bool mark = true) {
  for (int ar = 0; ar < 4; ++ar) {
    int newX = pixel->x + around[ar][0];
    int newY = pixel->y + around[ar][1];

    if (newX < 0 || newX >= 50 || newY < 0 || newY >= 50)
      continue;

    Pixel *newPixel = &grid[newY][newX];

    if (newPixel->c != c) {
      if (findContinue && newPixel->c == '*') {
        continuePixels.emplace_back(pixel);
      }
      continue;
    }

    if (newPixel->state != State::Undiscovered)
      continue;

    if (mark)
      newPixel->state = State::Discovered;

    stack.push(newPixel);
  }
}

int getDots(int x, int y) {
  int count = 0;

  Stack items;
  getSuroundingPixels(&grid[y][x], items, 'X',
                      false); // get all items and test them one by one

  while (!items.empty()) {
    findContinue = true;

    Pixel *pixel = items.top();
    items.pop();

    if (pixel->state != State::Undiscovered)
      continue;

    ++count;

    dfs(pixel->x, pixel->y, 'X');
  }

  return count;
}

int dfs(int x, int y, char c) {
  int total = 0;

  Pixel *pixel = &grid[y][x];

  if (pixel->state != State::Undiscovered || pixel->c != c)
    return 0;

  Stack stack;
  stack.push(pixel);
  pixel->state = State::Discovered;

  while (!stack.empty()) {
    Pixel *pixel = stack.top();
    stack.pop();
    pixel->state = State::Processed;

    getSuroundingPixels(pixel, stack, c);

    if (c == '*') {
      total += getDots(pixel->x, pixel->y);
      findContinue = false;

      if (continuePixels.size() > 0) {
        for (auto foundPixel : continuePixels) {
          stack.push(foundPixel);
        }

        continuePixels.clear();
      }
    }
  }

  return total;
}

std::vector<int> process() {
  std::vector<int> cubes;

  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < cols; ++x) {

      int nHoles = dfs(x, y, '*');
      if (nHoles > 0) {
        cubes.emplace_back(nHoles);
      }
    }
  }

  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < cols; ++x) {
      Pixel &pixel = grid[y][x];
      if (pixel.c == 'X' && pixel.state == State::Undiscovered) {
        dfs(pixel.x, pixel.y, 'X');
        cubes.emplace_back(1);
      }
    }
  }

  std::sort(cubes.begin(), cubes.end());
  return cubes;
}

int main() {
  int n = 0;
  while (true) {
    ++n;
    scanf("%d %d\n", &cols, &rows);
    if (rows == 0 && cols == 0)
      break;

    for (int y = 0; y < rows; ++y) {
      char row[52];
      fgets(row, 52, stdin);

      row[strlen(row) - 1] = 0;

      int len = strlen(row);
      if (len <= 0)
        continue;

      for (int x = 0; x < len; ++x) {
        grid[y][x].state = State::Undiscovered;
        grid[y][x].c = row[x];
        grid[y][x].x = x;
        grid[y][x].y = y;
      }
    }

    auto cubes = process();
    // Throw N
    printf("Throw %d\n", n);
    //  N N N N
    for (size_t i = 0; i < cubes.size(); ++i) {
      if (i > 0)
        putchar(' ');
      printf("%d", cubes[i]);
    }

    putchar('\n');
    putchar('\n');
  }
  return 0;
}
