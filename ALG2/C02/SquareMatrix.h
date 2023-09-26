#pragma once

#include <vector>

class SquareMatrix
{
private:
    std::vector<std::vector<double>> A;

public:
    // constructors
    SquareMatrix(const int size);
    SquareMatrix(const std::vector<std::vector<double>> data);
    SquareMatrix(const SquareMatrix &other);

    // print
    void report() const;

    // get + set
    double getAt(const int row, const int col) const;
    void setAt(const int row, const int col, const double value);

    double det() const;

    // el. operations
};