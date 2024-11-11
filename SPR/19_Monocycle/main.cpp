#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#define MAX_SIZE 25

enum Color { GREEN, WHITE, BLUE, RED, BLACK };

enum Side { NOTH, EAST, SOUTH, WEST };

struct Wheel {
  Color color = Color::GREEN;
  Side side = Side::NOTH;
  int x;
  int y;
};

static char grid[MAX_SIZE][MAX_SIZE];

int main() {
  while (true) {
    int w, h;
    scanf("%d %d\n", &w, &h);

    if (w == 0 && h == 0)
      break;

    Wheel wheel;

    for (int l = 0; l < h; ++l) {
      if (!fgets(grid[l], sizeof(grid[l]), stdin))
        return EXIT_FAILURE;
      int len = strlen(grid[l]);
      for (int i = 0; i < len; ++i) {
        if (grid[l][i] == 'S') {
          wheel.x = i;
          wheel.y = l;
        }
      }
    }
  }
  return 0;
}
