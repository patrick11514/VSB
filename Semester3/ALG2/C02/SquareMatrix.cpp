#include "SquareMatrix.h"
#include <iostream>

SquareMatrix::SquareMatrix(const int size)
{
    std::vector<std::vector<double>> a(size, std::vector<double>(size));

    for (int i = 0; i < size; i++)
    {
        a[i][i] = 1;
    }

    this->A = a;
}

SquareMatrix::SquareMatrix(const std::vector<std::vector<double>> data)
{
    if (data.size() == 0)
    {
        throw "Matrix does not have size";
    }

    if (data.size() != data[0].size())
    {
        throw "Matrix is not Square";
    }

    this->A = data;
}

SquareMatrix::SquareMatrix(const SquareMatrix &other)
{
    this->A = other.A;
}

void SquareMatrix::report() const
{
    std::size_t size = this->A.size();
    for (std::size_t i = 1; i <= size; i++)
    {
        for (std::size_t l = 1; l <= size; l++)
        {
            std::cout << this->getAt(i, l) << " ";
        }
        std::cout << "\n";
    }
}

int SquareMatrix::Order() const
{
    return this->A.size();
}

double SquareMatrix::getAt(const int row, const int col) const
{
    if (row - 1 > this->A.size() || row - 1 < 0)
    {
        return 0;
    }
    if (col - 1 > this->A.size() || col - 1 < 0)
    {
        return 0;
    }
    return this->A[row - 1][col - 1];
}

void SquareMatrix::setAt(const int row, const int col, const double value)
{
    if (row - 1 > this->A.size() || row - 1 < 0)
    {
        return;
    }
    if (col - 1 > this->A.size() || col - 1 < 0)
    {
        return;
    }
    this->A[row - 1][col - 1] = value;
}

double SquareMatrix::det() const
{
    SquareMatrix tmp = *this;

    int order = tmp.Order();
    int size = order;

    // Prvni řád
    if (order == 1)
    {
        return tmp.getAt(1, 1);
    }

    // Druhý řád
    if (order == 2)
    {
        return tmp.getAt(1, 1) * tmp.getAt(2, 2) - tmp.getAt(2, 1) * tmp.getAt(1, 2);
    }

    // Třetí řád
    if (order == 3)
    {
        return tmp.getAt(1, 1) * tmp.getAt(2, 2) * tmp.getAt(3, 3) + tmp.getAt(1, 2) * tmp.getAt(2, 3) * tmp.getAt(3, 1) + tmp.getAt(1, 3) * tmp.getAt(2, 1) * tmp.getAt(3, 2) - tmp.getAt(1, 3) * tmp.getAt(2, 2) * tmp.getAt(3, 1) - tmp.getAt(1, 2) * tmp.getAt(2, 1) * tmp.getAt(3, 3) - tmp.getAt(1, 1) * tmp.getAt(2, 3) * tmp.getAt(3, 2);
    }

    // Ostatní řády
    double det = 1;

    // Gausova eliminace
    for (int baseCol = 1; baseCol < size; baseCol++)
    {
        double base = tmp.getAt(baseCol, baseCol);

        for (int modifiedRow = baseCol + 1; modifiedRow <= size; modifiedRow++)
        {
            tmp.addRow(modifiedRow, baseCol, (tmp.getAt(modifiedRow, baseCol) / base) * -1.0);
        }
    }

    for (int i = 1; i <= size; i++)
    {
        det *= tmp.getAt(i, i);
    }

    return det;
}

void SquareMatrix::swapRows(const int row1, const int row2)
{
    if (row1 - 1 < 0 || row1 - 1 > this->A.size())
        return;
    if (row2 - 1 < 0 || row2 - 1 > this->A.size())
        return;

    this->A[row1 - 1].swap(this->A[row2 - 1]);
}

void SquareMatrix::multiplyRow(const int row, const double multiplier)
{
    if (row - 1 < 0 || row - 1 > this->A.size())
        return;

    for (std::size_t i = 0; i < this->A.size(); i++)
    {
        this->A[row - 1][i] *= multiplier;
    }
}

void SquareMatrix::addRow(const int row1, const int row2, const double multiplier)
{
    if (row1 - 1 < 0 || row1 - 1 > this->A.size())
        return;
    if (row2 - 1 < 0 || row2 - 1 > this->A.size())
        return;

    for (std::size_t i = 0; i < this->A.size(); i++)
    {
        this->A[row1 - 1][i] += this->A[row2 - 1][i] * multiplier;
    }
}
