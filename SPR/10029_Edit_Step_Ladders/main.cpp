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
    if (first.substr(1) == second) {
      return true;
    }
    return false;
  }

  if (first == second.substr(1)) {
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

void reset() {
  for (auto &point : points) {
    point.status = Status::UNVISITED;
    point.distance = 0;
  }
}

using Path = std::vector<Point *>;

void dfs(Point *begin) {
  std::stack<Point *> stack;
  begin->status = Status::VISITED;
  stack.push(begin);

  while (!stack.empty()) {
    auto front = stack.top();
    stack.pop();

    front->status = Status::PROCESSED;

    for (auto point : dict[front->word]) {
      if (point->status != Status::UNVISITED)
        continue;
      point->status = Status::VISITED;

      stack.push(point);
    }
  }
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
    dfs(&point);
    reset();

    int newMax = getMax();

    if (newMax > max) {
      max = newMax;
    }
  }

  printf("%d\n", max);
}

int main() {
  points.reserve(25143);

  // load dict
  char buf[255] = {0};
  while (fgets(buf, 255, stdin)) {
    buf[strlen(buf) - 1] = '\0';

    if (strlen(buf) == 0) {
      break;
    }

    insert(std::string{buf});
  }

  for (auto &pair : dict) {
    printf("%s\n", pair.first.c_str());
    for (auto &str : pair.second) {
      printf("- %s\n", str->word.c_str());
    }
  }

  solve();
  return 0;
}
