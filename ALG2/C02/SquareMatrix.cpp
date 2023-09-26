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
    for (std::size_t i = 0; i < size; i++)
    {
        for (std::size_t l = 0; l < size; l++)
        {
            std::cout << this->getAt(i, l) << " ";
        }
        std::cout << "\n";
    }
}

double SquareMatrix::getAt(const int row, const int col) const
{
    if (row > this->A.size() || col > this->A.size())
    {
        return 0;
    }
    return this->A[row][col];
}

void SquareMatrix::setAt(const int row, const int col, const double value)
{
    if (row > this->A.size() || col > this->A.size())
    {
        return;
    }
    this->A[row][col] = value;
}

double SquareMatrix::det() const
{
    SquareMatrix tmp = *this;
}
