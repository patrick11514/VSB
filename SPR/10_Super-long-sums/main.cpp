#include <cstdio>
#include <vector>

static char buffer[4];

int main() {
  int summs;
  scanf("%d\n", &summs);

  for (int i = 0; i < summs; ++i) {

    if (i > 0)
      putchar('\n');

    int digits;
    scanf("\n%d\n", &digits);

    std::vector<int> summed(digits, 0);

    for (int d = 0; d < digits; ++d) {
      fread(buffer, sizeof(char), 4, stdin);

      summed[d] = buffer[0] - '0' + buffer[2] - '0';

      for (int dBack = d; dBack >= 0; --dBack) {
        if (summed[dBack] < 10)
          break;

        int carry = summed[dBack] / 10;
        summed[dBack] = summed[dBack] % 10;
        summed[dBack - 1] += carry;
      }
    }

    for (auto digit : summed) {
      putchar('0' + digit);
    }
    putchar('\n');
  }
  return 0;
}
