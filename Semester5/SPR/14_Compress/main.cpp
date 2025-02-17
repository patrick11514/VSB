#include <algorithm>
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define BUFFER_SIZE 1024
#define ENCODED_SIZE 26 + 26 + 5

static int base_encoded[ENCODED_SIZE];

int getIndex(char c) {
  constexpr int FIRST_OFFSET = 'Z' - 'A' + 1;
  constexpr int SECOND_OFFSET = FIRST_OFFSET + 'z' - 'a';

  if (c >= 'A' && c <= 'Z')
    return c - 'A';
  else if (c >= 'a' && c <= 'z')
    return c - 'a' + FIRST_OFFSET;
  else if (c == ' ')
    return 1 + SECOND_OFFSET;
  else if (c == '.')
    return 2 + SECOND_OFFSET;
  else if (c == ',')
    return 3 + SECOND_OFFSET;
  else if (c == '-')
    return 4 + SECOND_OFFSET;
  else if (c == '$')
    return 5 + SECOND_OFFSET;
  return 127;
}

int countBits(int num) {
  int count = 0;
  while (num > 0) {
    num = num >> 1;
    ++count;
  }
  return count;
}

int solve(char lines[][BUFFER_SIZE], int *lengths, int lineCount) {
  int encoded[ENCODED_SIZE];
  std::copy(base_encoded, base_encoded + ENCODED_SIZE, encoded);

  std::pair<int, int> frequency[ENCODED_SIZE];

  // check frequency
  for (int l = 0; l < lineCount; ++l) {
    for (int i = 0; i < lengths[l]; ++i) {
      int c = getIndex(lines[l][i]);
      frequency[c].first = c;
      frequency[c].second++;
    }
  }

  std::sort(frequency, frequency + ENCODED_SIZE,
            [](const std::pair<char, int> &a, const std::pair<char, int> &b) {
              return a.second > b.second;
            });

  // go through characters and code them

  int prefix = 0b11;
  int counter = 0;

  for (int i = 0; i < ENCODED_SIZE; ++i) {
    std::pair<char, int> pair = frequency[i];
    int c = pair.first;
    if (encoded[c] != -1)
      continue;

    encoded[c] = counter++;

    if ((counter & 0b11) == 0b11) {
      counter = ((counter & 0xFFFC) << 2) | (prefix << 2);
    }
  }

  // code text

  int bits = 0;

  for (int l = 0; l < lineCount; ++l) {
    for (int i = 0; i < lengths[l]; ++i) {
      int c = getIndex(lines[l][i]);
      bits += countBits(encoded[c]);
    }
  }

  return bits;
}

int main() {
  // fill with -1
  std::fill(base_encoded, base_encoded + ENCODED_SIZE, -1);

  int encodings;
  scanf("%d\n", &encodings);

  for (int i = 0; i < encodings; ++i) {
    int lines;
    scanf("%d\n", &lines);

    char buffer[lines][BUFFER_SIZE];
    int lengths[lines];

    for (int l = 0; l < lines; ++l) {
      fgets(buffer[l], BUFFER_SIZE, stdin);

      // remove new-line
      int len = strlen(buffer[l]);
      buffer[l][len - 1] = '\0';
      lengths[l] = len - 1;
    }

    int result = solve(buffer, lengths, lines);
    printf("Result: %d\n", result);
  }

  return EXIT_SUCCESS;
}
