#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#define MAX_STATIONS 100
#define TANK 200

struct GasStation {
  int price;
  int distance;
};

static int distance;
static int stationCount;
static GasStation stations[MAX_STATIONS];

int solve() {
  std::vector<std::vector<int>> calculations(MAX_STATIONS + 1,
                                             std::vector<int>(TANK, INT_MAX));

  calculations[0][TANK / 2] = 0; // we have currently TANK / 2 fuel, and station
                                 // 0 => start, station 1 => station 0 etc...

  for (int i = 0; i < stationCount; ++i) {
    GasStation station = stations[i];

    int distance = stations[i + 1].distance - station.distance;

    for (int fuel = 0; fuel <= TANK; ++fuel) {
    }
  }
}

int main() {
  int cases;
  scanf("%d\n", &cases);

  for (int i = 0; i < cases; ++i) {
    stationCount = 0;

    getchar();

    scanf("%d\n", &distance);

    char buffer[255];

    while (fgets(buffer, sizeof(buffer), stdin)) {
      int price, distance;
      sscanf(buffer, "%d %d\n", &distance, &price);
      stations[stationCount++] = GasStation{price, distance};
    }

    solve();
  }

  return 0;
}
