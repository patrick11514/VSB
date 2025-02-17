#include <iostream>

/*
 * This file demonstrates variadic templates, which can be used to implement various complex
 * templated structures, and also functions with an arbitrary amount of parameters.
 */

// Base case - return 0
// This is basically a template specialization of the function below with variadic templates.
auto sum() {
    return 0;
}

// Function with variadic "Template parameter pack" -> ...T means that it accepts an arbitrary
// amount of arguments (including zero arguments).
// The unpacking of the template arguments in this function basically corresponds to `(x: xs)` in Haskell.
template<typename T1, typename ...T>
auto sum(T1 s, T ...ts){
    // ts... "unpacks" the arguments and generates another version of this function that will be
    // called with one less template argument.
    // It is called "fold expression". In this case, it is a "unary" fold expression, which repeats
    // the arguments separated with a comma.
    return s + sum(ts...);
}

int maximum() {
    return 0;
}

template<typename T1, typename ...T>
auto maximum(T1 a, T ...ts){
    return std::max(a, maximum(ts...));
}

template<typename ...Args>
auto sum_fold(Args ...args)
{
    // This is a binary fold expression, which repeats the arguments separated with + (the operation
    // around ...), and uses an initialization value `0`, for the case where there would be zero
    // template arguments.
    return (args + ... + 0);
}

template<typename ...Args>
void print(Args ...args) {
    (std::cout << ... << args) << "\n";
}

// Variadic templates can also be used for structures.
// This is a variadic structure that basically reimplements `std::tuple`.
template <class... Ts> struct tuple {};

template <class T, class... Ts>
struct tuple<T, Ts...> : tuple<Ts...> {
    tuple(T t, Ts... ts) : tuple<Ts...>(ts...), tail(t) {}

    T tail;
};

/*
Expands into (more or less):
```
struct tuple<double, uint64_t, const char*> : tuple<uint64_t, const char*> {
  double tail;
}

struct tuple<uint64_t, const char*> : tuple<const char*> {
  uint64_t tail;
}

struct tuple<const char*> : tuple {
  const char* tail;
}

struct tuple {
}
```
 */

// These structures and functions below implement the get method for tuples, using recursive
// variadic template evaluation.
template <size_t, class> struct elem_type_holder;

template <class T, class... Ts>
struct elem_type_holder<0, tuple<T, Ts...>> {
    using type = T;
};

template <size_t k, class T, class... Ts>
struct elem_type_holder<k, tuple<T, Ts...>> {
    using type = typename elem_type_holder<k - 1, tuple<Ts...>>::type;
};

template <size_t k, class... Ts>
typename std::enable_if<
        k == 0, typename elem_type_holder<0, tuple<Ts...>>::type&>::type
get(tuple<Ts...>& t) {
    return t.tail;
}

template <size_t k, class T, class... Ts>
typename std::enable_if<
        k != 0, typename elem_type_holder<k, tuple<T, Ts...>>::type&>::type
get(tuple<T, Ts...>& t) {
    tuple<Ts...>& base = t;
    return get<k - 1>(base);
}

// Type-based stack using variadics
// Note that the functions don't even have to be implemented - we only care about their signatures.
template <typename ...Ts> struct Stack {};

template <typename ...Ts, typename ...As>
constexpr auto push(Stack<Ts...>, As...) -> Stack<As..., Ts...>;

template <typename T, typename ...Ts>
constexpr auto pop(Stack<T, Ts...>) -> Stack<Ts...>;

template <typename T, typename ...Ts>
constexpr auto top(Stack<T, Ts...>) -> T;

template <typename T, typename Stack>
struct Contains {};

template <typename T>
struct Contains<T, Stack<>> {
    constexpr static bool Value = false;
};

template <typename T, typename ...Ts>
struct Contains<T, Stack<T, Ts...>> {
    constexpr static bool Value = true;
};

template <typename T, typename R, typename ...Ts>
struct Contains<T, Stack<R, Ts...>> {
    constexpr static bool Value = Contains<T, Stack<Ts...>>::Value;
};

template <typename Stack, typename Result>
struct Reverse {
    using Type = Result;
};

template <typename Result>
struct Reverse<Stack<>, Result> {
    using Type = Result;
};

template <typename T, typename ...Ts, typename ...Rs>
struct Reverse<Stack<T, Ts...>, Stack<Rs...>> {
    using Type = typename Reverse<Stack<Ts...>, Stack<T, Rs...>>::Type;
};

int main() {
    print(1, "2", true);

    auto x1 = sum(1, 2.3, 4.5f);
    print(x1);

    auto x2 = sum();
    print(x2);

    auto x3 = sum_fold(1, 2.3, 4.5f);
    print(x3);

    auto x4 = sum_fold();
    print(x4);

    auto x5 = maximum(1, 5, 4, 2, 8, 3);
    print(x5);

    tuple<int, bool> x6{15, true};
    std::cout << get<0>(x6) << std::endl;

    // using can be used to create "variables" in the world of types
    using Stack1 = Stack<>;                         // Stack<>
    using Stack2 = decltype(push(Stack1{}, 0));     // Stack<int>
    using Stack3 = decltype(push(Stack2{}, true));  // Stack<bool, int>

    using Stack4 = Stack<>;                         // Stack<>
    using Stack5 = decltype(push(Stack4{}, true));  // Stack<int>
    using Stack6 = decltype(push(Stack5{}, 0));     // Stack<bool, int>

    static_assert(Contains<int, Stack3>::Value);

    // With this type, we can reverse the stack
    Reverse<Stack3, Stack<>>::Type x = Stack6{};
    Stack<int, bool> reversed = x;

    return 0;
}
