#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

static int max = 0;
static std::map<std::string, int> dict;

void check(const std::string &str) {
  int localMax = 1;

  // iterate over string
  for (size_t i = 0; i <= str.size(); ++i) {
    std::string newStr{str};
    newStr.erase(i, 1);

    if (dict.count(newStr)) {
      localMax = std::max(dict[newStr] + 1, localMax);
    }

    for (char c = 'a'; c <= 'z'; ++c) {
      // swap
      if (i < str.size()) {
        std::string newStr{str};
        newStr[i] = c;

        if (newStr < str && dict.count(newStr)) {
          localMax = std::max(dict[newStr] + 1, localMax);
        }
      }

      // add
      std::string newStr{str};
      newStr.insert(i, 1, c);

      if (newStr < str && dict.count(newStr)) {
        localMax = std::max(dict[newStr] + 1, localMax);
      }
    }
  }

  dict[str] = localMax;
  if (localMax > max) {
    max = localMax;
  }
}

int main() {
  char buff[20];

  while (fgets(buff, 20, stdin)) {
    buff[strlen(buff) - 1] = 0;

    if (strlen(buff) == 0) {
      break;
    }

    check(buff);
  }

  printf("%d\n", max);
  return 0;
}
