#include <algorithm>
#include <cstdio>
#include <vector>

struct Elephant {
  int id;
  int weight;
  int iq;

  void print() { printf("%d. W:%d IQ:%d\n", this->id, this->weight, this->iq); }
};

static int count = 0;
static Elephant elephants[1000];

void solve() {
  std::sort(elephants, elephants + count,
            [](const Elephant &a, const Elephant &b) {
              return a.weight == b.weight ? a.iq > b.iq : a.weight < b.weight;
            });

  std::vector<int> sequence(count, -1);

  int totalHighest = 0;
  int highest[count];
  int max;

  for (int i = 0; i < count; ++i) {
    highest[i] = 1;

    for (int j = 0; j < i; ++j) {
      if (elephants[i].weight > elephants[j].weight &&
          elephants[i].iq < elephants[j].iq) {
        if (highest[i] < highest[j] + 1) {
          highest[i] = highest[j] + 1;
          sequence[i] = j;
        }
      }
    }

    if (highest[i] > totalHighest) {
      totalHighest = highest[i];
      max = i;
    }
  }

  std::vector<int> reversed;
  for (int i = max; i > -1; i = sequence[i]) {
    reversed.push_back(elephants[i].id);
  }

  std::reverse(reversed.begin(), reversed.end());

  printf("%d\n", totalHighest);

  for (const auto &id : reversed)
    printf("%d\n", id);
}

int main() {

  char buffer[255];
  while (fgets(buffer, sizeof(buffer), stdin)) {
    int weight, iq;
    sscanf(buffer, "%d %d\n", &weight, &iq);
    Elephant el{count + 1, weight, iq};
    elephants[count++] = el;
  }

  solve();
  return 0;
}
