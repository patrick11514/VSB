#pragma once

#include <array>
#include <iostream>

template <typename... Ts>
struct Stack
{
};

template <typename... Ts, typename... As>
constexpr auto push(Stack<Ts...>, As...) -> Stack<As..., Ts...>;

template <typename T, typename... Ts>
constexpr auto pop(Stack<T, Ts...>) -> Stack<Ts...>;

template <typename T, typename... Ts>
constexpr auto top(Stack<T, Ts...>) -> T;

// Matrix YAYYYY

template <typename T, size_t Rows, size_t Cols>
class Matrix
{
public:
    std::array<std::array<T, Cols>, Rows> data;

    Matrix() = default;

    constexpr Matrix(std::array<T, Rows * Cols> input)
    {
        for (size_t row = 0; row < Rows; ++row)
        {
            for (size_t col = 0; col < Cols; ++col)
            {
                this->data[row][col] = input[Cols * row + col];
            }
        }
    };

    constexpr const T &at(size_t row, size_t col) const
    {
        return this->data[row][col];
    }

    constexpr auto operator+(const Matrix<T, Rows, Cols> &other) const
    {
        Matrix<T, Rows, Cols> newMatrix;
        for (size_t row = 0; row < Rows; ++row)
        {
            for (size_t col = 0; col < Cols; ++col)
            {
                newMatrix.data[row][col] = this->data[row][col] + other.data[row][col];
            }
        }

        return newMatrix;
    }

    template <size_t newCols>
    constexpr auto operator*(const Matrix<T, Cols, newCols> &other) const
    {
        Matrix<T, Rows, newCols> newMatrix;
        for (size_t row = 0; row < Rows; ++row)
        {
            for (size_t col = 0; col < newCols; ++col)
            {
                size_t sum = 0;
                for (size_t currentCol = 0; currentCol < Cols; ++currentCol)
                {
                    sum += this->data[row][currentCol] * other.data[currentCol][col];
                }

                newMatrix.data[row][col] = sum;
            }
        }

        return newMatrix;
    }
};

// list types
class Null
{
};

template <int Value, typename... Ts>
class List
{
};

// list first
template <typename>
struct ListFirst;

template <int T, typename... Ts>
struct ListFirst<List<T, Ts...>>
{
    constexpr static int Value = T;
};

// list last

template <typename>
struct ListLast;

template <int T>
struct ListLast<List<T, Null>>
{
    constexpr static int Value = T;
};

template <int T, typename... Ts>
struct ListLast<List<T, Ts...>>
{
    constexpr static int Value = ListLast<Ts...>::Value;
};

// list length

template <typename>
struct ListLength;

template <>
struct ListLength<Null>
{
    constexpr static size_t Value = 0;
};

template <int T, typename... Ts>
struct ListLength<List<T, Ts...>>
{
    constexpr static size_t Value = ListLength<Ts...>::Value + 1;
};

// list sum

template <typename>
struct ListSum;

template <>
struct ListSum<Null>
{
    constexpr static size_t Value = 0;
};

template <int T, typename... Ts>
struct ListSum<List<T, Ts...>>
{
    constexpr static size_t Value = ListSum<Ts...>::Value + T;
};

// list max

template <typename>
struct ListMax;

template <int T, typename... Ts>
struct ListMax<List<T, Ts...>>
{
    constexpr static size_t Value = ListMax<Ts...>::Value > T ? ListMax<Ts...>::Value : T;
};

template <int T>
struct ListMax<List<T, Null>>
{
    constexpr static size_t Value = T;
};

// list reverse

template <typename>
struct ListReverse;

template <>
struct ListReverse<Null>
{
    using Value = Null;
};

template <int T>
struct ListReverse<List<T, Null>>
{
    using Value = List<T, Null>;
};

template <typename, typename>
struct ListReverseHelper;

template <int T, typename Inner>
struct ListReverseHelper<List<T, Null>, Inner>
{
    using Value = List<T, Inner>;
};

template <int T, typename... Ts, typename Inner>
struct ListReverseHelper<List<T, Ts...>, Inner>
{
    using Value = ListReverseHelper<Ts..., List<T, Inner>>::Value;
};

template <int T, typename... Ts>
struct ListReverse<List<T, Ts...>>
{
    using Value = ListReverseHelper<Ts..., List<T, Null>>::Value;
};

// interpreter

template <size_t V>
struct Number;

template <bool V>
struct Bool;

template <typename, typename>
struct Interpret;

// Get final values from stack

template <size_t number>
struct Interpret<Stack<>, Stack<Number<number>>>
{
    constexpr static size_t Result = number;
};

template <bool value>
struct Interpret<Stack<>, Stack<Bool<value>>>
{
    constexpr static bool Result = value;
};

// Push number

template <size_t V>
struct PushNumber;

template <size_t number, typename... Is, typename... Vs>
struct Interpret<Stack<PushNumber<number>, Is...>, Stack<Vs...>>
{
    constexpr static auto Result = Interpret<Stack<Is...>, Stack<Number<number>, Vs...>>::Result;
};

// Push bool

template <bool V>
struct PushBool;

template <bool value, typename... Is, typename... Vs>
struct Interpret<Stack<PushBool<value>, Is...>, Stack<Vs...>>
{
    constexpr static auto Result = Interpret<Stack<Is...>, Stack<Bool<value>, Vs...>>::Result;
};

// Add

struct Add;

template <size_t first, size_t second, typename... Is, typename... Vs>
struct Interpret<Stack<Add, Is...>, Stack<Number<first>, Number<second>, Vs...>>
{
    constexpr static auto Result = Interpret<Stack<Is...>, Stack<Number<first + second>, Vs...>>::Result;
};

// Max

struct Max;

template <size_t first, size_t second, typename... Is, typename... Vs>
struct Interpret<Stack<Max, Is...>, Stack<Number<first>, Number<second>, Vs...>>
{
    constexpr static auto Result = Interpret<Stack<Is...>, Stack<Number<(first > second ? first : second)>, Vs...>>::Result;
};

// Pop

struct Pop;

template <bool value, typename... Is, typename... Vs>
struct Interpret<Stack<Pop, Is...>, Stack<Bool<value>, Vs...>>
{
    constexpr static auto Result = Interpret<Stack<Is...>, Stack<Vs...>>::Result;
};

template <size_t value, typename... Is, typename... Vs>
struct Interpret<Stack<Pop, Is...>, Stack<Number<value>, Vs...>>
{
    constexpr static auto Result = Interpret<Stack<Is...>, Stack<Vs...>>::Result;
};

// Compare

struct Compare;

template <size_t first, size_t second, typename... Is, typename... Vs>
struct Interpret<Stack<Compare, Is...>, Stack<Number<first>, Number<second>, Vs...>>
{
    constexpr static auto Result = Interpret<Stack<Is...>, Stack<Bool<first == second>, Vs...>>::Result;
};

// Select

struct Select;

template <typename First, typename Second, typename... Is, typename... Vs>
struct Interpret<Stack<Select, First, Second, Is...>, Stack<Bool<true>, Vs...>>
{
    constexpr static auto Result = Interpret<Stack<First, Is...>, Stack<Vs...>>::Result;
};

template <typename First, typename Second, typename... Is, typename... Vs>
struct Interpret<Stack<Select, First, Second, Is...>, Stack<Bool<false>, Vs...>>
{
    constexpr static auto Result = Interpret<Stack<Second, Is...>, Stack<Vs...>>::Result;
};