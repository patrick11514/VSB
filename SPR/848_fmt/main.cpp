#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

static int len = 0;
static char buffer[5120];
static char line[1024];

void processData() {
  for (int i = 0; i < len; ++i) {
    char c = buffer[i];
    if (c == '\n' &&
        ((i > 0 && buffer[i - 1] != '\n') || buffer[i + 1] != '\n')) {
      buffer[i] = ' ';
    }
  }
}

int main() {
  while (fgets(line, 1024, stdin)) {
    if (strlen(line) < 72) {
      strcpy(buffer + len, line);

      len = strlen(buffer);
      // processData();
      continue;
    }
  }

  printf("%s\n", buffer);

  return 0;
}
