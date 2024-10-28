#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <unordered_map>

static char wordBuffer[12000];
static char subBuffer[12000];
static std::unordered_map<std::string, int> cache;

void print(size_t word, size_t subword) {
  printf("===================\n");
  printf("%s\n", wordBuffer);
  for (size_t i = 0; i < word; ++i) {
    putchar(' ');
  }
  printf("I\n");

  printf("%s\n", subBuffer);
  for (size_t i = 0; i < subword; ++i) {
    putchar(' ');
  }
  printf("I\n");
  printf("===================\n");
}

int findSubsesquences(char *word, size_t len, char *sub, size_t subLen,
                      int subs = 0) {
  int conLen = 0;
  for (size_t i = 0, l = 0; i < len;) {
    char c = word[i];
    char cS = sub[l];

    print(i, l);

    if (c == cS && l < subLen) {
      ++i;
      ++conLen;
    }
    if ((c != cS && l > 0 && conLen > 0) || (i >= len)) {
      printf("ADDED SUBS\n");
      ++subs;
      l = -1;
      conLen = 0;
    }

    ++l;
  }

  return subs;
}

int main() {
  int testCases;
  scanf("%d\n", &testCases);

  for (int i = 0; i < testCases; ++i) {
    fgets(wordBuffer, sizeof(wordBuffer), stdin);
    wordBuffer[strlen(wordBuffer) - 1] = '\0';
    fgets(subBuffer, sizeof(subBuffer), stdin);
    subBuffer[strlen(subBuffer) - 1] = '\0';

    int result = findSubsesquences(wordBuffer, strlen(wordBuffer), subBuffer,
                                   strlen(subBuffer));
    printf("RESULT: %d\n", result);
  }

  return 0;
}
