#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

enum Status { UNVISITED, CURRENT, VISITED };

struct Node {
  Status status;
  int length;
  std::vector<Node *> childs;
};

std::vector<Node *> find(Node *nameNode, Node *endNode) {
  std::queue<Node *> queue;
  std::vector<Node *> toReset{nameNode};

  queue.push(nameNode);
  nameNode->length = 0;

  while (!queue.empty()) {
    auto *front = queue.front();
    front->status = Status::VISITED;
    queue.pop();

    for (auto *child : front->childs) {
      if (child->status != Status::UNVISITED)
        continue;

      child->length = front->length + 1;
      child->status = Status::CURRENT;

      toReset.emplace_back(child);

      if (child == endNode) {
        return toReset; // we already got end, so we can end it
      }

      queue.push(child);
    }
  }

  return toReset;
}

int main() {
  int scenarios;
  scanf("%d\n", &scenarios);
  for (int i = 0; i < scenarios; ++i) {
    printf("Scenario %d\n", i + 1);
    int books, authors;
    scanf("%d %d\n", &books, &authors);

    std::unordered_map<std::string, Node> nodes;

    for (int i = 0; i < books; ++i) {
      char buffer[255];
      if (!fgets(buffer, sizeof(buffer), stdin))
        break;

      std::string slice{buffer};

      std::vector<std::string> names;

      // find authors
      bool end = false;
      while (!end) {
        auto next = slice.find(".,");
        if (next == slice.npos) {
          auto colon = slice.find(':');
          if (colon == slice.npos)
            return EXIT_FAILURE;
          next = colon - 1;
          end = true;
        }

        std::string name{slice.begin(), slice.begin() + next + 1};

        auto find = nodes.find(name);
        if (find == nodes.end()) {
          // add new
          Node node;
          node.childs = std::vector<Node *>{};
          node.length = -1;
          node.status = Status::UNVISITED;
          nodes.emplace(name, node);
        }

        names.emplace_back(name);

        slice = std::string{slice.begin() + next + 3, slice.end()};
      }

      for (size_t i = 0; i < names.size(); ++i) {
        auto *key = &nodes[names[i]];

        for (size_t l = 0; l < names.size(); ++l) {
          if (i == l)
            continue;

          key->childs.emplace_back(&nodes[names[l]]);
        }
      }
    }

    Node *end = &nodes["Erdos, P."];

    for (int i = 0; i < authors; ++i) {
      char buffer[255];
      if (!fgets(buffer, sizeof(buffer), stdin))
        return EXIT_FAILURE;

      buffer[strlen(buffer) - 1] = '\0'; // remove n-line

      std::string name{buffer};
      auto toReset = find(&nodes[name], end);

      if (end->length == -1) {
        printf("%s %s\n", name.c_str(), "infinity");
      } else {
        printf("%s %d\n", name.c_str(), end->length);
      }

      for (auto *ptr : toReset) {
        ptr->length = -1;
        ptr->status = Status::UNVISITED;
      }
    }
  }
  return EXIT_SUCCESS;
}
