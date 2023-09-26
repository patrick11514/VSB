#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "SquareMatrix.h"

int main()
{

    SquareMatrix a(10);
    a.setAt(2, 2, 2);
    a.report();

    SquareMatrix b(20);
    for (int i = 0; i < 5; i++)
    {
        b.setAt(i, i + 2, i * 2);
    }

    b.report();

    return 0;
}
