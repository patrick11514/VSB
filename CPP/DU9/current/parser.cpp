#include <array>
#include <cstddef>

/*
 * This file combines variadic templates, integral templates, constexpr to create a simple
 * compile-time expression evaluator.
 */

template <typename ...Ts> struct Stack {};

template <typename ...Ts, typename ...As>
constexpr auto push(Stack<Ts...>, As...) -> Stack<As..., Ts...>;

template <typename T, typename ...Ts>
constexpr auto pop(Stack<T, Ts...>) -> Stack<Ts...>;

template <typename T, typename ...Ts>
constexpr auto top(Stack<T, Ts...>) -> T;

template <size_t N>
struct FixedString {
    constexpr FixedString(const char (&data)[N]) {
        for (size_t i = 0; i < N; i++) {
            chars[i] = data[i];
        }
    }

    constexpr char operator[](size_t index) const { return chars[index]; }
    constexpr size_t size() const { return N; }

    std::array<char, N> chars;
};

template <size_t N>
struct Digit {
    constexpr static int value = N;
};

template <char C>
struct Char {
    constexpr static char value = C;
};

template <char C>
struct Operator {
    constexpr static char value = C;
};

struct StringEnd{};

template <FixedString Str>
struct Parser {
    template <size_t Pos>
    constexpr static auto get_char() {
        if constexpr (Pos < Str.size()) {
            if constexpr(Str[Pos] == '\0') {
                return StringEnd{};
            }
            else if constexpr (Str[Pos] >= '0' && Str[Pos] <= '9') {
                return Digit<Str[Pos] - '0'>{};
            }
            else if constexpr (Str[Pos] == '+' || Str[Pos] == '-' || Str[Pos] == '*') {
                return Operator<Str[Pos]>{};
            } else return Char<Str[Pos]>{};
        }
        else return StringEnd{};
    }

    template <size_t N1, size_t N2>
    constexpr static auto compute(Digit<N1>, Digit<N2>, Operator<'+'>) -> Digit<N1 + N2>;
    template <size_t N1, size_t N2>
    constexpr static auto compute(Digit<N1>, Digit<N2>, Operator<'-'>) -> Digit<N1 - N2>;
    template <size_t N1, size_t N2>
    constexpr static auto compute(Digit<N1>, Digit<N2>, Operator<'*'>) -> Digit<N1 * N2>;

    template <size_t Index, typename S>
    constexpr static int eval(S stack, StringEnd) {
        using next_value = decltype(top(stack));
        return next_value::value;
    }
    template <size_t Index, typename S, size_t N>
    constexpr static int eval(S stack, Digit<N> a) {
        using next_stack = decltype(push(stack, a));
        return parse<Index + 1>(next_stack{});
    }
    template <size_t Index, typename S, char C>
    constexpr static int eval(S stack, Operator<C> op) {
        using a = decltype(top(stack));
        using s1 = decltype(pop(stack));

        // Here we need to go from type to value to "call" top -> we can use std::declval or just
        // construct an object using {}.
        using b = decltype(top(std::declval<s1>()));
        using s2 = decltype(pop(s1{}));

        using result = decltype(compute(a{}, b{}, op));

        using next_stack = decltype(push(s2{}, result{}));
        return parse<Index + 1>(next_stack{});
    }

    template <size_t Index, typename S>
    constexpr static int parse(S stack) {
        using next_char = decltype(get_char<Index>());
        return eval<Index>(stack, next_char{});
    }

    constexpr static int Result = parse<0>(Stack<>{});
};

int main() {
    Parser<"34+5*"> p1;
    static_assert(p1.Result == 35);

    Parser<"32*"> p2;
    static_assert(p2.Result == 6);

//    Parser<"*"> p3; // parse error
//    static_assert(p3.Result == 0);

    return 0;
}
