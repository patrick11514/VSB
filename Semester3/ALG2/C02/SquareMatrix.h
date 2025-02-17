#pragma once

#include <vector>

/// @brief SquareMatrix class
class SquareMatrix
{
private:
    /// @brief Matrix data
    std::vector<std::vector<double>> A;

public:
    /**
     * @brief Constructs a square matrix of a given size.
     *
     * @param size The size of the square matrix.
     */
    SquareMatrix(const int size);
    /**
     * @brief Constructs a square matrix from a 2D vector of doubles.
     *
     * @param data A 2D vector of doubles representing the matrix data.
     */
    SquareMatrix(const std::vector<std::vector<double>> data);
    /**
     * @class SquareMatrix copy constructor.
     */
    SquareMatrix(const SquareMatrix &other);

    /**
     * Prints a report of the square matrix to the console.
     */
    void report() const;

    /**
     * Returns the order of the square matrix.
     *
     * @return The order of the square matrix.
     */
    int Order() const;

    /**
     * Returns the value at the specified row and column of the square matrix.
     *
     * @param row The row of the element to retrieve.
     * @param col The column of the element to retrieve.
     * @return The value at the specified row and column of the square matrix.
     */
    double getAt(const int row, const int col) const;

    /**
     * Sets the value of the element at the specified row and column indices.
     *
     * @param row The row of the element to set.
     * @param col The column of the element to set.
     * @param value The value to set the element to.
     */
    void setAt(const int row, const int col, const double value);

    /**
     * Calculates the determinant of the square matrix.
     *
     * @return The determinant of the square matrix.
     */
    double det() const;

    /**
     * Swaps two rows in the square matrix.
     *
     * @param row1 The of the first row to swap.
     * @param row2 The of the second row to swap.
     */
    void swapRows(const int row1, const int row2);
    /**
     * Multiplies the specified row of the matrix by the given multiplier.
     *
     * @param row The of the row to be multiplied.
     * @param multiplier The value to multiply the row by.
     */
    void multiplyRow(const int row, const double multiplier);
    /**
     * Adds the values of row2 to row1 in the matrix.
     *
     * @param row1 The of the row to add to.
     * @param row2 The of the row to add from.
     * @param multiplier The value to multiply the row by.
     */
    void addRow(const int row1, const int row2, const double multiplier);
};