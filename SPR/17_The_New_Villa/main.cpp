#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <vector>

struct Room {
  int id;
  std::vector<int> connections;
  std::vector<int> switches;
};

using intVec = std::vector<int>;

void solve(intVec &roomPairs, intVec &switches) {}

int main() {
  int n = 1;
  while (true) {
    int numberOfRooms;
    int doorCount;
    int switchCount;
    scanf("%d %d %d", &numberOfRooms, &doorCount, &switchCount);

    if (numberOfRooms == 0 && doorCount == 0 && switchCount == 0) {
      break;
    }

    intVec roomsPairs(numberOfRooms);

    for (int i = 0; i < numberOfRooms; ++i) {
      int from, to;

      scanf("%d %d\n", &from, &to);

      roomsPairs[from - 1] = to - 1;
    }

    intVec switches(switchCount);

    for (int i = 0; i < switchCount; ++i) {
      int from, to;

      scanf("%d %d\n", &from, &to);

      switches[from - 1] = to - 1;
    }

    printf("Villa #%d\n", n);

    ++n;
  }
  return 0;
}
