#include <array>
#include <cstdio>

static int coins[]{50, 25, 10, 5, 1};

void print(std::array<int, 5> &solution) {
  for (auto item : solution) {
    printf("%d ", item);
  }
  printf("\n");
}

int sumCents(std::array<int, 5> &solution) {
  int sum = 0;
  for (int i = 0; i < 5; ++i) {
    sum += solution[i] * coins[i];
  }
  return sum;
}

int solve(int cents, std::array<int, 5> &solution, int index, int count = 0) {
  if (index == 4) {
    return count + 1;
  }

  std::array<int, 5> copy{solution};

  while (true) {
    print(copy);
    if (sumCents(copy) > cents) {
      return count;
    }

    count = solve(cents, copy, index + 1, count);

    copy[index]++;
  }

  return count;
}

int main() {

  while (true) {
    int cents;
    int read = scanf("%d\n", &cents);
    if (read < 1)
      break;

    std::array<int, 5> solution{0, 0, 0, 0, 0};

    int ways = solve(cents, solution, 0);

    if (ways > 1) {
      printf("There are %d ways to produce %d cents change.\n", ways, cents);

    } else {
      printf("There is only 1 way to produce %d cents change.\n", cents);
    }
  }
  return 0;
}
