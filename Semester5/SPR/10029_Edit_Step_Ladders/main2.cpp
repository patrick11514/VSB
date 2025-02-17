#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

enum Status { UNVISITED, VISITED, PROCESSED };

struct Point {
  std::string word;
  Status status = Status::UNVISITED;
  int distance = 0;

  Point(const std::string &word) : word(word) {}
};

static std::vector<Point> points;
static std::unordered_map<std::string, std::vector<Point *>> dict;

struct StringSlice {
private:
  const std::string &str;
  int start;
  size_t len;

public:
  StringSlice(const std::string &str, int start)
      : str(str), start(start), len(str.size() - start) {}

  StringSlice(const std::string &str, int start, int len)
      : str(str), start(start), len(len < 0 ? str.size() + len : len) {}

  bool operator==(const StringSlice &right) {
    if (this->len != right.len)
      return false;
    for (size_t i = 0; i < this->len; ++i) {
      if ((this->str.c_str() + this->start)[i] !=
          (right.str.c_str() + right.start)[i])
        return false;
    }
    return true;
  }
  bool operator==(const std::string &right) {
    if (this->len != right.size())
      return false;
    for (size_t i = 0; i < this->len; ++i) {
      if ((this->str.c_str() + this->start)[i] != right[i])
        return false;
    }
    return true;
  }

  friend bool operator==(const std::string &left, const StringSlice &right) {
    if (left.size() != right.len)
      return false;
    for (size_t i = 0; i < right.len; ++i) {
      if ((right.str.c_str() + right.start)[i] != left[i])
        return false;
    }
    return true;
  }
};

bool isTransformation(const std::string &first, const std::string &second) {
  if (std::abs(static_cast<int>(first.size()) -
               static_cast<int>(second.size())) > 1)
    return false;

  if (first.size() == second.size()) {
    int diff = 0;
    for (size_t i = 0; i < first.size(); ++i) {
      if (first[i] == second[i])
        continue;

      ++diff;

      if (diff > 1)
        return false;
    }

    if (diff == 0)
      return false;
    return true;
  }

  // handle the missing character case
  if (first.size() > second.size()) {
    if (StringSlice(first, 1) == second) {
      return true;
    }

    if (StringSlice(first, 0, -1) == second) {
      return true;
    }

    return false;
  }

  if (first == StringSlice(second, 1)) {
    return true;
  }
  if (first == StringSlice(second, 0, -1)) {
    return true;
  }
  return false;
}

void insert(const std::string &toInsert) {
  points.emplace_back(toInsert);

  if (points.size() == 0) {
    return;
  }

  for (auto &point : points) {
    if (!isTransformation(point.word, toInsert))
      continue;

    dict[point.word].emplace_back(&points[points.size() - 1]);
    dict[toInsert].emplace_back(&point);
  }
}

Point *getPoint(const std::string &str) {
  for (auto &point : points) {
    if (point.word == str)
      return &point;
  }
  return nullptr;
}

void reset(std::vector<Point *> &resetPoints) {
  for (auto &point : resetPoints) {
    point->status = Status::UNVISITED;
    point->distance = 0;
  }
}

using Path = std::vector<Point *>;

std::vector<Point *> dfs(Point *begin) {
  std::vector<Point *> toReset;
  toReset.emplace_back(begin);

  std::stack<Point *> stack;
  begin->status = Status::VISITED;
  stack.push(begin);

  while (!stack.empty()) {
    auto top = stack.top();
    stack.pop();

    top->status = Status::PROCESSED;

    for (auto point : dict[top->word]) {
      if (point->status != Status::UNVISITED)
        continue;
      point->status = Status::VISITED;
      point->distance = top->distance + 1;

      toReset.emplace_back(point);

      stack.push(point);
    }
  }
  return toReset;
}

int getMax() {
  int max = 0;
  for (auto &point : points) {
    if (point.distance > max) {
      max = point.distance;
    }
  }

  return max;
}

void solve() {
  int max = 0;

  for (auto &point : points) {
    auto toReset = dfs(&point);

    int newMax = getMax();

    if (newMax > max) {
      max = newMax;
    }

    reset(toReset);
  }

  printf("%d\n", max);
}

int main() {
  points.reserve(25000);
  dict.reserve(25000);

  // load dict
  char buf[255] = {0};
  while (fgets(buf, 255, stdin)) {
    buf[strlen(buf) - 1] = '\0';

    if (strlen(buf) == 0) {
      break;
    }

    insert(std::string{buf});
  }

  printf("SOVING\n");

  solve();
  return 0;
}
