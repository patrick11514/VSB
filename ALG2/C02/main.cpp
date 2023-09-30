#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include "SquareMatrix.h"

int main()
{

    // create matrix
    int size = 10;

    SquareMatrix a(size);

    // fill matrix
    int numbers[] = {1, 2, -1, 2, 2, -2, 0, 2, -1, 2,
                     0, 3, 0, -3, 1, -1, -2, 3, -3, 0,
                     -2, -1, 0, -1, -1, -2, -2, -1, 2, 1,
                     0, 0, 3, 1, 3, 0, 3, 2, 0, -1,
                     1, 3, -3, 2, -3, 1, -2, 1, 2, -3,
                     3, 3, -2, 1, -1, -3, 1, 1, -3, 3,
                     2, -2, 2, 2, 3, -1, -2, -3, -1, 3,
                     -1, 0, -3, -2, 2, -1, 1, -1, 0, 0,
                     2, -3, 0, -3, -1, 3, -3, 1, -1, -1,
                     3, 0, 1, -3, 3, 3, -1, 3, -1, 1};

    for (int row = 1; row <= size; row++)
    {
        for (int col = 1; col <= size; col++)
        {
            a.setAt(row, col, numbers[(row - 1) * size + (col - 1)]);
        }
    }

    // show matrix
    a.report();

    std::cout << "\n";

    // calcualte determinant
    double det = a.det();
    std::cout << "Det: " << std::setprecision(10) << det << std::endl;

    return 0;
}
