#include <array>
#include <cstdio>
#include <unordered_map>

static int coins[]{50, 25, 10, 5, 1};

// For each layer 0-4 we have map, which caches the result
static std::array<std::unordered_map<int, int>, 5> cached;

void print(std::array<int, 5> &solution) {
  for (auto item : solution) {
    printf("%d ", item);
  }
  printf("\n");
}

int sumCents(std::array<int, 5> &solution, int index = 0) {
  int sum = 0;
  for (int i = index; i < 5; ++i) {
    sum += solution[i] * coins[i];
  }
  return sum;
}

int solve(int cents, std::array<int, 5> solution, int index, int count = 0) {
  printf("Cents: %d, Index %d, Count: %d\n", cents, index, count);
  if (index == 4) {
    solution[index] = cents - sumCents(solution, index);
    print(solution);
    return count + 1;
  }

  auto res = cached[index].find(cents);
  if (res != cached[index].end()) {
    printf("Loaded: %d \n", res->second);
    print(solution);
    return res->second + count;
  }

  int rawCount = 0;

  while (true) {
    if (sumCents(solution, index) > cents) {
      break;
    }

    rawCount = count;

    count =
        solve(cents - sumCents(solution, index), solution, index + 1, count);

    solution[index]++;
  }
  if (cents > 0) {
    printf("Saving %d coins index %d count %d\n", cents, index, count);
    cached[index].emplace(cents, count - rawCount);
  }

  printf("%d coins have %d ways, index: %d\n", cents, count, index);
  return count;
}

int main() {
  while (true) {
    int cents;
    int read = scanf("%d\n", &cents);
    if (read < 1)
      break;

    int ways = solve(cents, {0, 0, 0, 0, 0}, 0);

    if (ways > 1) {
      printf("There are %d ways to produce %d cents change.\n", ways, cents);

    } else {
      printf("There is only 1 way to produce %d cents change.\n", cents);
    }
  }
  return 0;
}
