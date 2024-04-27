// This tells doctest to provide a main() function
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "tasks.h"

// Checks that T::value does not compile
template<typename T>
constexpr std::enable_if_t<T::value, bool> HasValue() {
    return true;
}

template <typename T>
constexpr bool HasValue() {
    return false;
}

// Checks that T::Result does not compile
template<typename T>
constexpr std::enable_if_t<T::Result, bool> HasResult() {
    return true;
}

template <typename T>
constexpr bool HasResult() {
    return false;
}

TEST_SUITE("Matrix") {
    TEST_CASE("Indexing") {
        constexpr Matrix<int, 3, 2> a{{1, 2, 3, 4, 5, 6}};
        static_assert(a.at(0, 0) == 1);
        static_assert(a.at(0, 1) == 2);
        static_assert(a.at(1, 0) == 3);
        static_assert(a.at(1, 1) == 4);
        static_assert(a.at(2, 0) == 5);
        static_assert(a.at(2, 1) == 6);
    }

    TEST_CASE("Addition") {
        constexpr Matrix<int, 3, 2> a{{1, 2, 3, 4, 5, 6}};
        constexpr Matrix<int, 3, 2> b{{2, 1, 0, 3, 4, 8}};

        constexpr Matrix<int, 3, 2> c = a + b;

        static_assert(c.at(0, 0) == 3);
        static_assert(c.at(0, 1) == 3);
        static_assert(c.at(1, 0) == 3);
        static_assert(c.at(1, 1) == 7);
        static_assert(c.at(2, 0) == 9);
        static_assert(c.at(2, 1) == 14);
    }

    TEST_CASE("Multiplication same dimensions") {
        constexpr Matrix<int, 2, 2> a{{1, 2, 3, 4}};
        constexpr Matrix<int, 2, 2> b{{2, 1, 0, 3}};

        constexpr Matrix<int, 2, 2> c = a * b;

        static_assert(c.at(0, 0) == 2);
        static_assert(c.at(0, 1) == 7);
        static_assert(c.at(1, 0) == 6);
        static_assert(c.at(1, 1) == 15);
    }

    TEST_CASE("Multiplication different dimensions") {
        constexpr Matrix<int, 4, 3> a{{1, 2, 3, 4, 2, 1, 0, 1, 3, 5, 6, 4}};
        constexpr Matrix<int, 3, 2> b{{2, 1, 0, 3, 1, 2}};

        constexpr Matrix<int, 4, 2> c = a * b;

        static_assert(c.at(0, 0) == 5);
        static_assert(c.at(0, 1) == 13);
        static_assert(c.at(1, 0) == 9);
        static_assert(c.at(1, 1) == 12);
        static_assert(c.at(2, 0) == 3);
        static_assert(c.at(2, 1) == 9);
        static_assert(c.at(3, 0) == 14);
        static_assert(c.at(3, 1) == 31);
    }
}
TEST_SUITE("Linked List") {
    TEST_CASE("First") {
        using MyList = List<1, List<2, List<3, List<4, Null>>>>;
        static_assert(ListFirst<MyList>::Value == 1);
        static_assert(ListFirst<List<5, Null>>::Value == 5);
        static_assert(!HasValue<ListFirst<Null>>());
    }

    TEST_CASE("Last") {
        using MyList = List<1, List<2, List<3, List<4, Null>>>>;
        static_assert(ListLast<MyList>::Value == 4);
        static_assert(ListLast<List<5, Null>>::Value == 5);
        static_assert(!HasValue<ListLast<Null>>());
    }

    TEST_CASE("Length") {
        using MyList = List<1, List<2, List<6, Null>>>;
        static_assert(ListLength<MyList>::Value == 3);
        static_assert(ListLength<List<5, Null>>::Value == 1);
        static_assert(ListLength<Null>::Value == 0);
    }

    TEST_CASE("Sum") {
        using MyList = List<1, List<2, List<6, Null>>>;
        static_assert(ListSum<MyList>::Value == 9);
        static_assert(ListSum<List<5, Null>>::Value == 5);
        static_assert(ListSum<Null>::Value == 0);
    }

    TEST_CASE("Max") {
        using MyListA = List<1, List<2, List<6, Null>>>;
        static_assert(ListMax<MyListA>::Value == 6);

        using MyListB = List<1, List<12, List<6, List<3, Null>>>>;
        static_assert(ListMax<MyListB>::Value == 12);

        using MyListC = List<100, List<12, List<6, Null>>>;
        static_assert(ListMax<MyListC>::Value == 100);

        static_assert(ListMax<List<5, Null>>::Value == 5);
        static_assert(!HasValue<ListMax<Null>>());
    }

    TEST_CASE("Reverse") {
        static_assert(std::is_same_v<ListReverse<Null>::Value, Null>);
        static_assert(std::is_same_v<ListReverse<List<1, Null>>::Value, List<1, Null>>);
        static_assert(std::is_same_v<ListReverse<List<2, List<1, Null>>>::Value, List<1, List<2, Null>>>);
    }
}
TEST_SUITE("Interpreter") {
    TEST_CASE("PushNumber") {
        using Program = Stack<PushNumber<5>>;
        static_assert(Interpret<Program, Stack<>>::Result == 5);
    }
    TEST_CASE("PushBool true") {
        using Program = Stack<PushBool<true>>;
        static_assert(Interpret<Program, Stack<>>::Result == true);
    }
    TEST_CASE("PushBool false") {
        using Program = Stack<PushBool<false>>;
        static_assert(Interpret<Program, Stack<>>::Result == false);
    }
    TEST_CASE("Add") {
        using Program = Stack<PushNumber<1>, PushNumber<2>, Add>;
        static_assert(Interpret<Program, Stack<>>::Result == 3);
        static_assert(!HasResult<Interpret<Stack<Add>, Stack<>>>());
    }
    TEST_CASE("Max") {
        using Program = Stack<PushNumber<2>, PushNumber<5>, PushNumber<3>, Max, Max>;
        static_assert(Interpret<Program, Stack<>>::Result == 5);
        static_assert(!HasResult<Interpret<Stack<Max>, Stack<>>>());
    }
    TEST_CASE("Pop 1") {
        using Program = Stack<PushNumber<2>, PushNumber<5>, Pop>;
        static_assert(Interpret<Program, Stack<>>::Result == 2);
    }
    TEST_CASE("Pop 2") {
        using Program = Stack<PushNumber<5>, PushNumber<2>, Pop>;
        static_assert(Interpret<Program, Stack<>>::Result == 5);
    }
    TEST_CASE("Pop 3") {
        using Program = Stack<PushNumber<5>, PushNumber<2>, PushNumber<10>, Pop, Pop>;
        static_assert(Interpret<Program, Stack<>>::Result == 5);
    }
    TEST_CASE("Pop 4") {
        using Program = Stack<PushBool<true>, PushBool<false>, Pop>;
        static_assert(Interpret<Program, Stack<>>::Result == true);
        static_assert(!HasResult<Interpret<Stack<Pop>, Stack<>>>());
    }
    TEST_CASE("Compare true") {
        using Program = Stack<PushNumber<2>, PushNumber<2>, Compare>;
        static_assert(Interpret<Program, Stack<>>::Result == true);
        static_assert(!HasResult<Interpret<Stack<Compare>, Stack<>>>());
    }
    TEST_CASE("Compare false") {
        using Program = Stack<PushNumber<1>, PushNumber<2>, Compare>;
        static_assert(Interpret<Program, Stack<>>::Result == false);
    }
    TEST_CASE("Select true") {
        using Program = Stack<PushBool<true>, Select, PushNumber<1>, PushNumber<2>, PushNumber<3>, Add>;
        static_assert(Interpret<Program, Stack<>>::Result == 4);
    }
    TEST_CASE("Select false") {
        using Program = Stack<PushBool<false>, Select, PushNumber<1>, PushNumber<2>, PushNumber<3>, Add>;
        static_assert(Interpret<Program, Stack<>>::Result == 5);
    }

    TEST_CASE("Complex program") {
        using Program = Stack<
                PushNumber<5>,
                PushNumber<6>,
                Compare,
                Select,
                PushNumber<1>,
                PushNumber<2>,
                PushNumber<3>,
                Add
        >;

        static_assert(Interpret<Program, Stack<>>::Result == 5);
    }
}
