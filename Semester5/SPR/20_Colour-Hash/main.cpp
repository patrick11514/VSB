#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <set>
#include <string>
#include <vector>

static int puzzle[24];
static const int final[24] = {0, 3, 4, 3, 0, 5, 6,  5, 0, 1, 2, 1,
                              0, 7, 8, 7, 0, 9, 10, 9, 0, 1, 2, 1};

enum Status {
  UNVISITED,
  CURRENT,
  VISITED,
};

struct State {
  int puzzle[24];
  int length = 0;
  Status state = Status::UNVISITED;

  State(int *puzzle) { std::copy(puzzle, puzzle + 24, this->puzzle); };
  State(const State &old) {
    std::copy(old.puzzle, old.puzzle + 24, this->puzzle);
  }
  State(State &&old) { std::swap(old.puzzle, this->puzzle); }

  void shiftRight(int start, int end) {
    int prev = -1;
    for (int i = start; i < end; ++i) {
      int index = start + (i % end);

      int tmp = this->puzzle[index];

      if (prev > -1) {
        this->puzzle[index] = prev;
      }

      prev = tmp;
    }

    this->puzzle[start] = prev;
  }

  void shiftLeft(int start, int end) {
    int prev = -1;
    for (int i = end - 1; i >= 0; --i) {
      int index = start + (i % end);

      int tmp = this->puzzle[index];

      if (prev > -1) {
        this->puzzle[index] = prev;
      }

      prev = tmp;
    }

    this->puzzle[end - 1] = prev;
  }

  State leftClockWise() {
    State newState(*this);
    // modify
    newState.shiftRight(0, 12);
    newState.shiftRight(0, 12);

    return newState;
  }

  State leftAntiClockWise() {
    State newState(*this);
    // modify
    newState.shiftLeft(0, 12);
    newState.shiftLeft(0, 12);

    return newState;
  }

  State rightClockWise() {
    State newState(*this);
    // modify
    newState.shiftRight(0, 12);
    newState.shiftRight(0, 12);

    return newState;
  }

  State rightAntiClockWise() {
    State newState(*this);
    // modify
    newState.shiftLeft(0, 12);
    newState.shiftLeft(0, 12);

    return newState;
  }

  std::string toString() const {
    std::string str;
    str.reserve(24);
    for (int i = 0; i < 24; ++i)
      str += std::to_string(puzzle[i]);
    return str;
  }

  bool isFinal() {
    for (int i = 0; i < 24; ++i) {
      if (this->puzzle[i] != final[i]) {
        return false;
      }
    }

    return true;
  }
  void print() {
    for (int i = 0; i < 24; ++i) {
      printf("%d ", this->puzzle[i]);
    }
    printf("\n");
  }
};

void solve() {
  std::queue<State> stack;
  State begin(puzzle);
  stack.push(begin);

  while (!stack.empty()) {
  }
}

int main() {

  int puzzle[24]{0, 3, 4, 3, 0, 5, 6,  5, 0, 1, 2, 1,
                 0, 7, 8, 7, 0, 9, 10, 9, 0, 1, 2, 1};

  State test(puzzle);
  test.print();
  printf("%d\n", test.isFinal());

  test.leftClockWise().leftAntiClockWise().print();
  /*int puzzles;
  scanf("%d\n", &puzzles);
  for (int i = 0; i < puzzles; ++i) {
    for (int l = 0; l < 24; ++l) {
      scanf("%d", &puzzle[l]);
    }

    solve();
  }*/
  return 0;
}
