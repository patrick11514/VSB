#include <cstdio>
#include <vector>

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
      int n1, n2;

      scanf("%d %d\n", &n1, &n2);

      summed[d] = n1 + n2;

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
