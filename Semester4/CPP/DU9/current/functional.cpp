/*
 * This file shows how we can use recursive template evaluation to perform calculations.
 * Compile-time computation using templates is performed using types - instead of computing with
 * values, you compute with types and their type arguments (which might be either other types or
 * values).
 *
 * You can "program" with templates using declarative structs and template arguments. "If conditions"
 * (e.g. stopping condition for a recursion) can be implemented using specialization.
 *
 * Below, the compiler will generate the necessary types to perform an exponentiation operation
 * - check the expanded result on https://cppinsights.io.
 */

template<int N, int P>
struct Pow {
    constexpr static int value = N * Pow<N, P - 1>::value;
};

// Pattern matching using specialization - stop recursion at P=0.
template<int N>
struct Pow<N, 0> {
    constexpr static int value = 1;
};

template <int N>
struct Factorial {
    constexpr static int Result = N * Factorial<N - 1>::Result;
};

template <>
struct Factorial<0> {
    constexpr static int Result = 1;
};

int main() {
    Pow<2, 8> x1;

    // decltype: value -> type
    // std::declval: type -> value
    static_assert(decltype(x1)::value == 256);
    static_assert(x1.value == 256);

    // Pow<2, -1> x2;

    return 0;
}

