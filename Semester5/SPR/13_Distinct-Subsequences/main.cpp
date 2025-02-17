#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>

class CustomNumber {
private:
  std::vector<short> digits;

public:
  CustomNumber() : digits({0}) {};
  CustomNumber(unsigned int init) {
    while (init >= 1) {
      this->digits.emplace_back(init % 10);
      init /= 10;
    }
  };

  void print() const {
    for (size_t i = this->digits.size(); i > 0; --i) {
      putchar('0' + this->digits[i - 1]);
    }
  }

  CustomNumber &operator+=(const CustomNumber &other) {
    if (other.digits.size() >= this->digits.size()) {
      // resize for 1 digit in reserve for overflow
      this->digits.reserve(other.digits.size() + 1);
    }

    short carry = 0;
    for (size_t i = 0; i < other.digits.size() || carry > 0; ++i) {
      short sum = carry;

      if (this->digits.size() > i)
        sum += this->digits[i];

      if (other.digits.size() > i)
        sum += other.digits[i];

      if (carry > 0)
        carry = 0;
      if (sum >= 10) {
        carry = 1;
        sum -= 10;
      }

      if (i < this->digits.size())
        this->digits[i] = sum;
      else
        this->digits.emplace_back(sum);
    }

    return *this;
  }

  friend CustomNumber operator+(CustomNumber lhs, const CustomNumber &rhs) {
    lhs += rhs;
    return lhs;
  }
};

static char wordBuffer[12000];
static char subBuffer[12000];

void findSubsesquences(char *word, size_t len, char *sub, size_t subLen) {
  CustomNumber summs[(subLen + 1) * 2];

  // fill init
  for (size_t i = 0; i <= subLen; ++i) {
    summs[i] = i > 0 ? 0 : 1;
  }

  size_t firstStart = 0;
  size_t secondStart = subLen + 1;

  for (size_t i = 0; i < len; ++i) {
    char c = word[i];

    for (size_t l = 0; l <= subLen; ++l) {
      char c2;
      if (l == 0)
        c2 = '\0';
      else
        c2 = sub[l - 1];

      if (c == c2)
        summs[secondStart + l] =
            summs[firstStart + l - 1] + summs[firstStart + l];
      else
        summs[secondStart + l] = summs[firstStart + l];
    }

    std::swap(firstStart, secondStart);
  }

  summs[(subLen + 1) * 2 - 1].print();
}

int main() {
  int testCases;
  scanf("%d\n", &testCases);

  for (int i = 0; i < testCases; ++i) {
    fgets(wordBuffer, sizeof(wordBuffer), stdin);
    wordBuffer[strlen(wordBuffer) - 1] = '\0';
    fgets(subBuffer, sizeof(subBuffer), stdin);
    subBuffer[strlen(subBuffer) - 1] = '\0';

    findSubsesquences(wordBuffer, strlen(wordBuffer), subBuffer,
                      strlen(subBuffer));
    printf("\n");
  }

  return 0;
}
