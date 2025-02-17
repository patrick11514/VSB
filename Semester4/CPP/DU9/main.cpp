#include "tasks.h"

int main()
{
    constexpr Matrix<int, 3, 4> a{{1,2,3,4,5,6}};
    constexpr Matrix<int, 4,2>b{{1,2,3,4,5,6}};

    constexpr auto c = a * b;
}
