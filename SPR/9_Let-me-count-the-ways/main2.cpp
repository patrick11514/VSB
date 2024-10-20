#include <array>
#include <cstdio>
#include <unordered_map>

static int coins[]{50, 25, 10, 5, 1};

// For each layer 0-4 we have map, which caches the result
static std::array<std::unordered_map<int, long>, 5> cached;

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

long solve(int cents, std::array<int, 5> solution, int index, long count = 0) {
  if (index == 4) {
    return count + 1;
  }

  auto res = cached[index].find(cents);
  if (res != cached[index].end()) {
    return res->second + count;
  }

  long rawCount = 0;

  while (true) {
    if (sumCents(solution, index) > cents) {
      break;
    }

    long pre = count;
    count =
        solve(cents - sumCents(solution, index), solution, index + 1, count);

    // only add diffs, so we get raw count for this subset
    rawCount += count - pre;

    solution[index]++;
  }
  // if (cents > 0) {
  cached[index].emplace(cents, rawCount);
  //}

  return count;
}

int main() {
  while (true) {
    int cents;
    int read = scanf("%d\n", &cents);
    if (read < 1)
      break;

    long ways = solve(cents, {0, 0, 0, 0, 0}, 0);

    if (ways > 1) {
      printf("There are %ld ways to produce %d cents change.\n", ways, cents);

    } else {
      printf("There is only 1 way to produce %d cents change.\n", cents);
    }
  }
  return 0;
}
