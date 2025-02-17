#include <iostream>
#include <array>

/*
 * This file shows how you can use integral/value template arguments to model an array
 * with a compile-time size. You can use "pattern matching" in function signature to check
 * that e.g. some parameter has identical size as the current array (see `operator+`).
 * Or you can introduce a new template parameter and allow different sizes (see `join`).
 */

template <typename N, size_t size>
class StaticArray {
public:
    constexpr static size_t SIZE = size;

    StaticArray() = default;
    explicit constexpr StaticArray(const std::array<N, size>& items) {
        for (size_t i = 0; i < size; i++) {
            this->items[i] = items[i];
        }
    }
    explicit constexpr StaticArray(N value) {
        for (size_t i = 0; i < size; i++) {
            this->items[i] = value;
        }
    }
    template <size_t size2>
    constexpr StaticArray<N, size + size2> join(const StaticArray<N, size2>& other) {
        StaticArray<N, size + size2> result;
        for (size_t i = 0; i < size; i++) {
            result.items[i] = this->items[i];
        }
        for (size_t i = size; i < size2; i++) {
            result.items[size + i] = other.items[i];
        }
        return result;
    }

    template <size_t size2>
    constexpr StaticArray<N, size> operator+(const StaticArray<N, size2>& other) const {
        static_assert(size == size2, "size must equal");
        StaticArray<N, size> result;
        for (size_t i = 0; i < size; i++) {
            result.items[i] = this->items[i] + other.items[i];
        }
        return result;
    }
    constexpr StaticArray<N, size> operator+(const StaticArray<N, 1>& other) const {
        StaticArray<N, size> result;
        for (size_t i = 0; i < size; i++) {
            result.items[i] = this->items[i] + other.items[0];
        }
        return result;
    }

    constexpr N sum() const {
        N result{};
        for (size_t i = 0; i < size; i++) {
            result += this->items[i];
        }
        return result;
    }

    const N& operator[](size_t index) const {
        return this->items[index];
    }

    std::array<N, size> items = {};
};

template <typename N, size_t size>
std::ostream& operator<<(std::ostream& os, const StaticArray<N, size>& array) {
    for (size_t i = 0; i < size; i++) {
        std::cout << array[i] << ", ";
    }
    return os;
}

int main() {
    StaticArray<int, 3> a1;
    StaticArray<int, 4> a2;
    auto res1 = a1.join(a2);
    std::cout << decltype(res1)::SIZE << std::endl;

    StaticArray<int, 3> x1{{1, 2, 3}};
    StaticArray<int, 3> x2{{4, 5, 6}};
    auto res2 = x1 + x2;
    std::cout << res2 << std::endl;

    StaticArray<int, 4> x4{{4, 5, 6, 8}};
    StaticArray<int, 1> x5{4};
//    auto res_invalid = x1 + x4; // compile error
    auto res3 = x4 + x5;

    constexpr StaticArray<int, 4> x6{std::array{4, 1, 0, 2}};
    constexpr StaticArray<int, 4> x7{std::array{4, 3, 4, 7}};

    constexpr int res4 = (x6 + x7).sum(); // this is computed at compile-time
    static_assert(res4 == 25);

    return 0;
}
