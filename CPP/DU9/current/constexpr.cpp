#include <array>
#include <cstring>

/*
 * constexpr is a modifier for functions (or variables) that states that the
 * given function *may* be executed at compile time.
 *
 * Note that constexpr functions can be also executed at runtime.
 *
 * It can be used for imperative compile-time programming (instead of functional/recursive compile-time
 * programming that can be done using templates).
 *
 * Not all operations can be performed in constexpr functions that are executed in compile-time context,
 * of course (such as dynamic memory allocation).
 *
 * There is also the consteval modifier, which requires that the function *has* to be executable
 * at compile time.
 *
 * And there is also constinit :) Which says that a variable must have static initialization.
 */

constexpr int factorial(int n) {
    if (n < 3) return n;
    return n * factorial(n - 1);
}

constexpr int factorial2(int n) {
    int x = 1;
    for (int i = 2; i <= n; i++) {
        x *= i;
    }
    return x;
}

constexpr int parse(const char* str) {
    int nums[2] = { 0, 0 };
    int target = 0;
    char op = '\0';

    for (; *str != '\0'; str++) {
        char c = *str;
        if (c == ' ') continue;
        if (c >= '0' && c <= '9') {
            nums[target] = nums[target] * 10 + (c - '0');
        } else {
            if (op != '\0') return -1;
            op = c;
            target++;
        }
    }

    switch (op) {
        case '+': return nums[0] + nums[1];
        case '-': return nums[0] - nums[1];
        default: return -1;
    }
}

int main() {
    static_assert(factorial(5) == 120);
    static_assert(factorial2(5) == 120);

    static_assert(parse("13+24") == 37);
    static_assert(parse("+24") == 24);
//    static_assert(parse("+24") == 25);
    static_assert(parse("x+24") == -1);

    return 0;
}
