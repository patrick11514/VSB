// This tells doctest to provide a main() function
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <vector>
#include <sstream>

#include "doctest.h"
#include "tasks.h"

template <typename AnyIterable>
static auto to_vec(const AnyIterable &iter)
{
    using Type = std::decay_t<decltype(*iter.begin())>;

    std::vector<Type> items;
    for (auto item : iter)
    {
        items.push_back(item);
    }
    return items;
}

// Helper hack for nicer output in failed tests
namespace std
{
    template <typename T>
    std::ostream &operator<<(std::ostream &os, const std::vector<T> &array)
    {
        for (size_t i = 0; i < array.size(); i++)
        {
            os << array[i];
            if (i < array.size() - 1)
            {
                os << ", ";
            }
        }
        return os;
    }
}

TEST_SUITE("Array construction")
{
    TEST_CASE("Construct int array")
    {
        NumpyArray<int> arr{std::vector<int>{1, 2, 3}};
        REQUIRE(to_vec(arr) == std::vector<int>{1, 2, 3});
    }
    TEST_CASE("Construct float array")
    {
        NumpyArray<float> arr{std::vector<float>{1, 2, 3}};
        REQUIRE(to_vec(arr) == std::vector<float>{1, 2, 3});
    }
}

TEST_SUITE("Indexing")
{
    TEST_CASE("Read value by index")
    {
        std::vector<int> items{1, 2, 3};
        NumpyArray<int> arr{items};

        REQUIRE(arr[0] == 1);
        REQUIRE(arr[1] == 2);
        REQUIRE(arr[2] == 3);
    }
    TEST_CASE("Read value index too large")
    {
        NumpyArray<int> arr{{1, 2, 3, 4, 5, 6, 7, 8, 9}};
        REQUIRE_THROWS(arr[20]);
    }
    TEST_CASE("Change value by index")
    {
        NumpyArray<int> arr{{1, 2, 3, 4, 5, 6, 7, 8, 9}};
        arr[5] = 50;
        REQUIRE(arr[5] == 50);
    }
}

TEST_SUITE("Slice creation")
{
    TEST_CASE("Create slice wrong start index")
    {
        NumpyArray<float> arr{std::vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9}};
        REQUIRE_THROWS((void)arr.slice(50, 51));
    }
    TEST_CASE("Create slice wrong end index")
    {
        NumpyArray<float> arr{std::vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9}};
        REQUIRE_THROWS((void)arr.slice(0, 51));
    }
    TEST_CASE("Create slice crossed indices")
    {
        NumpyArray<float> arr{std::vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9}};
        REQUIRE_THROWS((void)arr.slice(4, 2));
    }
    TEST_CASE("Create slice")
    {
        NumpyArray<float> arr{std::vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9}};

        auto sub1 = arr.slice(1, 4);
        REQUIRE(sub1.size() == 3);
        REQUIRE(to_vec(sub1) == std::vector<float>{2, 3, 4});

        arr[1] = 100;
        REQUIRE(to_vec(sub1) == std::vector<float>{100, 3, 4});
    }
    TEST_CASE("Create slice from slice")
    {
        NumpyArray<float> arr{std::vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9}};

        auto sub1 = arr.slice(1, 4);
        auto sub2 = sub1.slice(1, 3);

        arr[2] = 100;
        REQUIRE(to_vec(sub2) == std::vector<float>{100, 4});
    }
    TEST_CASE("Create open-ended slice")
    {
        NumpyArray<float> arr{std::vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9}};

        auto sub1 = arr.slice(2);
        REQUIRE(to_vec(sub1) == std::vector<float>{3, 4, 5, 6, 7, 8, 9});

        auto sub2 = sub1.slice(3);
        REQUIRE(to_vec(sub2) == std::vector<float>{6, 7, 8, 9});
    }
    TEST_CASE("Create slice repeatedly")
    {
        NumpyArray<float> arr{std::vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9}};

        auto slice = arr.slice(0).slice(0).slice(0).slice(0);
        REQUIRE(to_vec(slice) == std::vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9});
    }
}

TEST_SUITE("Slice is view into original data")
{
    TEST_CASE("Change array after slice is created")
    {
        NumpyArray<float> arr{std::vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9}};

        auto sub = arr.slice(2);

        arr[3] = 9;

        REQUIRE(to_vec(sub) == std::vector<float>{3, 9, 5, 6, 7, 8, 9});
    }
}

TEST_SUITE("Iteration")
{
    TEST_CASE("Iterate const array")
    {
        std::vector<int> items{1, 2, 3};
        const NumpyArray<int> arr{items};

        int index = 0;
        for (auto item : arr)
        {
            REQUIRE(item == items[index++]);
        }
    }
    TEST_CASE("Iterate mutable array")
    {
        std::vector<int> items{1, 2, 3};
        NumpyArray<int> arr{items};

        int index = 0;
        for (auto item : arr)
        {
            REQUIRE(item == items[index++]);
        }
    }
    TEST_CASE("Iterate slice")
    {
        std::vector<int> items{1, 2, 3, 4, 5, 6, 7, 8};
        NumpyArray<int> arr{items};

        auto slice = arr.slice(2, 5);

        int index = 2;
        for (auto item : slice)
        {
            REQUIRE(item == items[index++]);
        }
    }
}

TEST_SUITE("Printing")
{
    TEST_CASE("Print array")
    {
        NumpyArray<int> arr{std::vector<int>{1, 2, 3, 4, 5}};

        std::stringstream ss;
        ss << arr;
        REQUIRE(ss.str() == "1, 2, 3, 4, 5");
    }
    TEST_CASE("Print slice")
    {
        NumpyArray<int> arr{std::vector<int>{1, 2, 3, 4, 5}};

        std::stringstream ss;
        ss << arr.slice(1, 3);
        REQUIRE(ss.str() == "2, 3");
    }
}

TEST_SUITE("Addition")
{
    TEST_CASE("Add scalar")
    {
        NumpyArray<int> arr{std::vector<int>{1, 2, 3, 4, 5}};

        REQUIRE(to_vec(arr + 5) == std::vector<int>{6, 7, 8, 9, 10});
        REQUIRE(to_vec(5 + arr) == std::vector<int>{6, 7, 8, 9, 10});
    }
    TEST_CASE("Add scalar to slice")
    {
        NumpyArray<int> arr{std::vector<int>{1, 2, 3, 4, 5}};
        auto slice = arr.slice(1, 4);
        auto result = slice + 5;
        REQUIRE(to_vec(result) == std::vector<int>{7, 8, 9});
        REQUIRE(to_vec(arr) == std::vector<int>{1, 2, 3, 4, 5});
    }
    TEST_CASE("Add array wrong size")
    {
        NumpyArray<int> arr1{std::vector<int>{1, 2, 3, 4, 5}};
        NumpyArray<int> arr2{std::vector<int>{1, 2, 3}};

        REQUIRE_THROWS(arr1 + arr2);
    }
    TEST_CASE("Add array broadcast left")
    {
        NumpyArray<int> arr1{std::vector<int>{1}};
        NumpyArray<int> arr2{std::vector<int>{1, 2, 3}};

        auto result = arr1 + arr2;
        REQUIRE(to_vec(result) == std::vector<int>{2, 3, 4});
    }
    TEST_CASE("Add array broadcast right")
    {
        NumpyArray<int> arr1{std::vector<int>{1}};
        NumpyArray<int> arr2{std::vector<int>{1, 2, 3}};

        auto result = arr2 + arr1;
        REQUIRE(to_vec(result) == std::vector<int>{2, 3, 4});
    }
    TEST_CASE("Add array with slice broadcast")
    {
        NumpyArray<int> arr1{std::vector<int>{1}};
        NumpyArray<int> arr2{std::vector<int>{1, 2, 3}};

        auto result = arr1 + arr2.slice(0, 3);
        REQUIRE(to_vec(result) == std::vector<int>{2, 3, 4});
    }
    TEST_CASE("Add array same size")
    {
        NumpyArray<int> arr1{std::vector<int>{1, 2, 3, 4, 5}};
        NumpyArray<int> arr2{std::vector<int>{3, 4, 2, 1, 8}};

        auto result = arr1 + arr2;
        REQUIRE(to_vec(result) == std::vector<int>{4, 6, 5, 5, 13});
    }
    TEST_CASE("Add array with slice")
    {
        NumpyArray<int> arr1{std::vector<int>{1, 2, 3, 4}};
        NumpyArray<int> arr2{std::vector<int>{5, 6, 7, 8, 9, 10}};

        auto slice = arr2.slice(1, 5);

        auto result = arr1 + slice;
        REQUIRE(to_vec(result) == std::vector<int>{7, 9, 11, 13});

        auto r2 = slice + arr1;
        REQUIRE(to_vec(result) == to_vec(r2));
    }
    TEST_CASE("Add slice with slice")
    {
        NumpyArray<int> arr1{std::vector<int>{1, 2, 3, 4, 5}};
        NumpyArray<int> arr2{std::vector<int>{5, 6, 7, 8, 9, 10}};

        auto v1 = arr1.slice(1, 4);
        auto v2 = arr2.slice(2, 5);

        auto result = v1 + v2;
        REQUIRE(to_vec(result) == std::vector<int>{9, 11, 13});

        auto r2 = v2 + v1;
        REQUIRE(to_vec(result) == to_vec(r2));
    }
}

TEST_SUITE("Multiplication")
{
    TEST_CASE("Multiply scalar")
    {
        NumpyArray<int> arr{std::vector<int>{1, 2, 3, 4, 5}};

        auto result = arr * 5;
        REQUIRE(to_vec(result) == std::vector<int>{5, 10, 15, 20, 25});
    }
    TEST_CASE("Multiply scalar to slice")
    {
        NumpyArray<int> arr{std::vector<int>{1, 2, 3, 4, 5}};
        auto slice = arr.slice(1, 4);
        auto result = slice * 5;
        REQUIRE(to_vec(result) == std::vector<int>{10, 15, 20});
    }
    TEST_CASE("Multiply array wrong size")
    {
        NumpyArray<int> arr1{std::vector<int>{1, 2, 3, 4, 5}};
        NumpyArray<int> arr2{std::vector<int>{1, 2, 3}};

        REQUIRE_THROWS(arr1 * arr2);
    }
    TEST_CASE("Multiply array broadcast left")
    {
        NumpyArray<int> arr1{std::vector<int>{2}};
        NumpyArray<int> arr2{std::vector<int>{1, 2, 3}};

        auto result = arr1 * arr2;
        REQUIRE(to_vec(result) == std::vector<int>{2, 4, 6});
    }
    TEST_CASE("Multiply array broadcast right")
    {
        NumpyArray<int> arr1{std::vector<int>{2}};
        NumpyArray<int> arr2{std::vector<int>{1, 2, 3}};

        auto result = arr2 * arr1;
        REQUIRE(to_vec(result) == std::vector<int>{2, 4, 6});
    }
    TEST_CASE("Multiply array with slice broadcast")
    {
        NumpyArray<int> arr1{std::vector<int>{2}};
        NumpyArray<int> arr2{std::vector<int>{1, 2, 3}};

        auto result = arr1 * arr2.slice(0, 3);
        REQUIRE(to_vec(result) == std::vector<int>{2, 4, 6});
    }
    TEST_CASE("Multiply array same size")
    {
        NumpyArray<int> arr1{std::vector<int>{1, 2, 3, 4, 5}};
        NumpyArray<int> arr2{std::vector<int>{3, 4, 2, 1, 8}};

        auto result = arr1 * arr2;
        REQUIRE(to_vec(result) == std::vector<int>{3, 8, 6, 4, 40});
    }
    TEST_CASE("Multiply array with slice")
    {
        NumpyArray<int> arr1{std::vector<int>{1, 2, 3, 4}};
        NumpyArray<int> arr2{std::vector<int>{5, 6, 7, 8, 9, 10}};

        auto slice = arr2.slice(1, 5);

        REQUIRE(to_vec(arr1 * slice) == std::vector<int>{6, 14, 24, 36});
        REQUIRE(to_vec(slice * arr1) == to_vec(arr1 * slice));
    }
    TEST_CASE("Multiply slice with slice")
    {
        NumpyArray<int> arr1{std::vector<int>{1, 2, 3, 4, 5}};
        NumpyArray<int> arr2{std::vector<int>{5, 6, 7, 8, 9, 10}};

        auto v1 = arr1.slice(1, 4);
        auto v2 = arr2.slice(2, 5);

        REQUIRE(to_vec(v1 * v2) == std::vector<int>{14, 24, 36});
        REQUIRE(to_vec(v2 * v1) == to_vec(v1 * v2));
    }
}

TEST_SUITE("Iterators")
{

    TEST_CASE("Iterable from vec")
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        Iterable it{vec.begin(), vec.end()};

        auto from_it = to_vec(it);

        REQUIRE_EQ(vec, from_it);
    }

    TEST_CASE("Iterable from array")
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        NumpyArray<int> arr{vec};

        auto from_it = to_vec(arr.iter());

        REQUIRE_EQ(vec, from_it);
    }

    TEST_CASE("Filter")
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        NumpyArray<int> arr{vec};

        auto filtered = arr
                            .iter()
                            .filter([](const int item)
                                    { return item % 2 == 0; });

        auto from_it = to_vec(filtered);

        REQUIRE_EQ(from_it, std::vector{2, 4});
    }

    TEST_CASE("Filter with state")
    {
        std::vector<int> vec{1, 2, 3, 4, 5, 6};
        NumpyArray<int> arr{vec};

        auto filtered = arr
                            .iter()
                            .filter([i = 0ull](int) mutable
                                    {
                        ++i;
                        return i % 2 == 0; });

        auto from_it = to_vec(filtered);

        REQUIRE_EQ(from_it, std::vector{2, 4, 6});
    }

    TEST_CASE("Map")
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        NumpyArray<int> arr{vec};

        auto mapped = arr
                          .iter()
                          .map([](const int item)
                               { return item * 2; });

        auto from_it = to_vec(mapped);

        REQUIRE_EQ(from_it, std::vector{2, 4, 6, 8, 10});
        REQUIRE_EQ(to_vec(arr), vec);
    }

    TEST_CASE("Map strings")
    {
        std::vector<std::string> vec{
            "Zadny easter egg zde nebude",
            "Jsem pod dohledem eticke komise"};
        NumpyArray<std::string> arr{vec};

        auto mapped = arr
                          .iter()
                          .map([](const std::string &item)
                               { return item.size(); });

        auto from_it = to_vec(mapped);

        REQUIRE_EQ(from_it, std::vector<size_t>{27, 31});
        REQUIRE_EQ(to_vec(arr), vec);
    }

    TEST_CASE("Reduce")
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        NumpyArray<int> arr{vec};

        int reduced = arr
                          .iter()
                          .reduce([](int &acc, const int item)
                                  { acc += item; });

        REQUIRE_EQ(reduced, 15);
    }

    TEST_CASE("Chain transformations")
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        NumpyArray<int> arr{vec};

        auto reduced = arr
                           .iter()
                           .filter([](const int item)
                                   { return item % 2 == 0; })
                           .map([](const int item)
                                { return item * 2; })
                           .reduce([](int &acc, const int item)
                                   { acc += item; });

        REQUIRE_EQ(reduced, 12);
        REQUIRE_EQ(to_vec(arr), vec);
    }

    TEST_CASE("Transformations are lazy")
    {

        size_t counter = 0;

        struct Predicate
        {

            size_t &c;

            Predicate(size_t &c) : c(c) {}

            bool operator()(int)
            {
                ++c;
                return true;
            }
        };

        std::vector<int> vec{1, 2, 3, 4, 5, 6};
        NumpyArray<int> arr{vec};

        Predicate pred(counter);

        auto filtered = arr
                            .iter()
                            .filter(pred);

        auto it = filtered.begin();
        ++it;
        ++it;
        ++it;

        // Predicate is applied on the first-fourth element when traversing from the first
        // to fourth element
        REQUIRE_EQ(counter, 4);
    }

    TEST_CASE("Transformations on an std::vector")
    {
        std::vector<int> nums{1, 2, 3, 4, 5, 6};

        Iterable iter{nums.begin(), nums.end()};

        auto mapped = iter.filter([](int n)
                                  { return n % 2 == 1; })
                          .map([](int n)
                               { return n * 2; });

        auto vec = to_vec(mapped);

        REQUIRE_EQ(vec, std::vector{2, 6, 10});
    }
}

// Tests for bonus functionality

TEST_SUITE("Multi-dimensional index")
{
    TEST_CASE("Shape count")
    {
        REQUIRE_THROWS(NumpyArray<int>{{1, 2, 3}, {}});
        REQUIRE_THROWS(NumpyArray<int>{{1, 2, 3}, {4}});
        REQUIRE_THROWS(NumpyArray<int>{{1, 2, 3}, {4, 2, 8}});
        NumpyArray<int> x1{{1, 2, 3}, {1, 1, 3}};
        NumpyArray<int> x2{{1, 2, 3}, {1, 3, 1}};
        NumpyArray<int> x3{{1, 2, 3}, {3, 1, 1}};
    }
    TEST_CASE("Wrong index count")
    {
        NumpyArray<int> items{{1, 2, 3}, {3}};
        REQUIRE_THROWS(items.nd_index1(std::array<size_t, 0>{}));
        REQUIRE_THROWS(items.nd_index1(std::array<size_t, 2>{0, 0}));
    }
    TEST_CASE("Wrong index value")
    {
        NumpyArray<int> items{{1, 2, 3, 4}, {2, 2}};
        REQUIRE_THROWS(items.nd_index1(std::array<size_t, 2>{2, 0}));
        REQUIRE_THROWS(items.nd_index1(std::array<size_t, 2>{0, 2}));
        REQUIRE_THROWS(items.nd_index1(std::array<size_t, 2>{2, 2}));
    }
    TEST_CASE("2D array 1")
    {
        NumpyArray<int> items{{1, 2, 3, 4, 5, 6, 7, 8}, {2, 4}};
        REQUIRE(items.nd_index1(std::array<size_t, 2>{0, 0}) == 1);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{0, 1}) == 2);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{0, 2}) == 3);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{0, 3}) == 4);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{1, 0}) == 5);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{1, 1}) == 6);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{1, 2}) == 7);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{1, 3}) == 8);
    }
    TEST_CASE("2D array 2")
    {
        NumpyArray<int> items{{1, 2, 3, 4, 5, 6, 7, 8}, {4, 2}};
        REQUIRE(items.nd_index1(std::array<size_t, 2>{0, 0}) == 1);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{0, 1}) == 2);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{1, 0}) == 3);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{1, 1}) == 4);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{2, 0}) == 5);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{2, 1}) == 6);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{3, 0}) == 7);
        REQUIRE(items.nd_index1(std::array<size_t, 2>{3, 1}) == 8);
    }
    TEST_CASE("3D array")
    {
        std::vector<int> data(24);
        // Fill with numbers 1, 2, 3, 4, ...
        std::iota(data.begin(), data.end(), 1);

        NumpyArray<int> items{data, {2, 4, 3}};

        int expected = 1;
        for (size_t i = 0; i < 2; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                for (size_t k = 0; k < 3; k++)
                {
                    std::array<size_t, 3> index{i, j, k};
                    REQUIRE(items.nd_index1(index) == expected);
                    expected++;
                }
            }
        }
    }
}

TEST_SUITE("&& specialization for boolean arrays")
{
    template <class T, class = decltype(std::declval<T>() && std::declval<T>())>
    std::true_type supports_and(const T &);
    std::false_type supports_and(...);

    template <class T>
    using supports_and_test = decltype(supports_and(std::declval<T>()));

    static_assert(supports_and_test<NumpyArray<bool>>());
    static_assert(!supports_and_test<NumpyArray<int>>());

    TEST_CASE("And")
    {
        NumpyArray<bool> b1{{true, false, true, false}};
        NumpyArray<bool> b2{{true, true, false, false}};

        auto result = b1 && b2;
        REQUIRE(to_vec(result) == std::vector<bool>{true, false, false, false});
    }
    TEST_CASE("And slice")
    {
        NumpyArray<bool> b1{{true, false, true, false}};
        NumpyArray<bool> b2{{true, true, false, false}};

        auto result = b1.slice(0, 4) && b2.slice(0, 4);
        REQUIRE(to_vec(result) == std::vector<bool>{true, false, false, false});
    }
    TEST_CASE("And scalar")
    {
        NumpyArray<bool> arr1{std::vector<bool>{true, true, false}};

        REQUIRE(to_vec(arr1 && true) == std::vector<bool>{true, true, false});
        REQUIRE(to_vec(false && arr1) == std::vector<bool>{false, false, false});
    }
    TEST_CASE("And array broadcast")
    {
        NumpyArray<bool> arr1{std::vector<bool>{true}};
        NumpyArray<bool> arr2{std::vector<bool>{true, true, false}};

        REQUIRE(to_vec(arr1 && arr2) == std::vector<bool>{true, true, false});
        REQUIRE(to_vec(arr2 && arr1) == std::vector<bool>{true, true, false});
    }
}
