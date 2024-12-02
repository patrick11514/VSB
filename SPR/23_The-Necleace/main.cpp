#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <vector>

#define COLORS 100

static std::vector<int> edges[COLORS];
static std::vector<int> path;

void clear() {
  path.clear();

  for (int i = 0; i < COLORS; ++i) {
    std::vector<int> vec;
    vec.reserve(COLORS);
    edges[i] = vec;
  }
}

/*bool solve() {
  std::stack<int> stack;

  int start;
  for (int i = 0; i < COLORS; ++i) {
    if (edges[i].size() == 0)
      continue;
    start = i;
    break;
  }

  stack.push(start);

  while (!stack.empty()) {

    int color = stack.top();

    path.emplace_back(color);
    stack.pop();

    auto &edgeList = edges[color];
    if (edgeList.size() == 0) {
      printf("Edge list for %d is empty\n", color);
      return false;
    }

    int edge = edgeList.back();

    edgeList.pop_back();

    // remove from the other side
    auto &other = edges[edge];
    other.erase(std::find(other.begin(), other.end(), color));

    stack.push(edge);
  }

  return true;
}*/

bool solve() {
  std::stack<int> stack;

  // Find starting vertex
  int start = -1, oddCount = 0;
  for (int i = 0; i < COLORS; ++i) {
    if (edges[i].size() % 2 != 0) {
      ++oddCount;
      start = i; // Start at a vertex with an odd degree
    }
  }

  if (oddCount != 0 && oddCount != 2)
    return false;    // Invalid Eulerian Path
  if (start == -1) { // If no odd vertex exists, pick any non-zero degree vertex
    for (int i = 0; i < COLORS; ++i) {
      if (!edges[i].empty()) {
        start = i;
        break;
      }
    }
  }

  stack.push(start);

  while (!stack.empty()) {
    int color = stack.top();

    // If the current vertex has edges, continue traversal
    if (!edges[color].empty()) {
      int next = edges[color].back(); // Pick the last edge
      edges[color].pop_back();        // Remove edge from this vertex

      // Remove the edge in the opposite direction
      auto &reverseEdge = edges[next];
      auto it = std::find(reverseEdge.begin(), reverseEdge.end(), color);
      if (it != reverseEdge.end())
        reverseEdge.erase(it);

      stack.push(next);
    } else {
      // If no more edges, add vertex to path and backtrack
      path.emplace_back(color);
      stack.pop();
    }
  }

  // Ensure all edges have been used
  for (int i = 0; i < COLORS; ++i) {
    if (!edges[i].empty())
      return false;
  }

  return true;
}

int main() {
  int cases;
  scanf("%d\n", &cases);
  for (int i = 0; i < cases; ++i) {
    if (i != 0)
      putchar('\n');

    printf("Case #%d\n", i + 1);

    int beads;
    scanf("%d\n", &beads);

    clear();

    for (int i = 0; i < beads; ++i) {
      int left, right;
      scanf("%d %d\n", &left, &right);
      // printf("Adding edge: %d -> %d\n", left, right);
      edges[left].emplace_back(right);
      edges[right].emplace_back(left);
    }

    if (solve()) {
      for (size_t i = 0; i < path.size() - 1; ++i) {
        printf("%d %d\n", path[i], path[i + 1]);
      }
    } else {
      printf("some beads may be lost\n");
    }
  }

  return 0;
}
