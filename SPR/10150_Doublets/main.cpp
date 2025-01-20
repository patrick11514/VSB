#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

enum Status { UNVISITED, VISITED, PROCESSED };

struct Point {
  std::string word;
  Status status = Status::UNVISITED;

  Point(const std::string &word) : word(word) {}
};

static std::vector<Point> points;
static std::unordered_map<std::string, std::vector<Point *>> dict;
static char first[18], second[18];

bool isDoublet(const std::string &first, const std::string &second) {
  if (first.size() != second.size())
    return false;

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

void insertDoublet(const std::string &toInsert) {
  points.emplace_back(toInsert);

  if (points.size() == 0) {
    return;
  }

  for (auto &point : points) {
    if (!isDoublet(point.word, toInsert))
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
  for (auto &point : points)
    point.status = Status::UNVISITED;
}

using Path = std::vector<Point *>;

void bfs() {
  auto begin = getPoint(first);
  auto end = getPoint(second);

  if (begin == nullptr || end == nullptr) {
    printf("No solution.\n");
    return;
  }

  std::queue<std::pair<Point *, Path>> stack;
  begin->status = Status::VISITED;
  stack.push({begin, {begin}});

  while (!stack.empty()) {
    auto front = stack.front();
    stack.pop();

    front.first->status = Status::PROCESSED;

    for (auto point : dict[front.first->word]) {
      if (point->status != Status::UNVISITED)
        continue;
      if (point == end) {
        front.second.emplace_back(end);
        for (auto &item : front.second) {
          printf("%s\n", item->word.c_str());
        }
        return;
      }

      point->status = Status::VISITED;

      auto path = front.second;
      path.emplace_back(point);

      stack.push({point, path});
    }
  }

  printf("No solution.\n");
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

    insertDoublet(std::string{buf});
  }

  int n = 0;

  // solution
  while (fgets(buf, 255, stdin)) {
    if (n > 0) {
      putchar('\n');
    }

    n++;
    sscanf(buf, "%s %s\n", first, second);
    bfs();
    reset();
  }

  return 0;
}
