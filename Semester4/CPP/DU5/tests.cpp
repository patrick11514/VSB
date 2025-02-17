// This tells doctest to provide a main() function
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "tasks.h"

// Old string tests from 3rd lesson
TEST_SUITE("UTF8String old")
{
    TEST_CASE("Create empty")
    {
        const UTF8String str;
        REQUIRE(str.get_byte_count() == 0);
        REQUIRE(str.get_point_count() == 0);
    }
    TEST_CASE("Create with string")
    {
        const UTF8String str{"hello"};
        REQUIRE(str.get_byte_count() == 5);
        REQUIRE(str.get_point_count() == 5);
    }
    TEST_CASE("Create with std::string")
    {
        const UTF8String str{std::string("hello")};
        REQUIRE(str.get_byte_count() == 5);
        REQUIRE(str.get_point_count() == 5);
    }
    TEST_CASE("Create with code points")
    {
        const UTF8String str{std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        }};
        REQUIRE(str.get_byte_count() == 11);
        REQUIRE(str.get_point_count() == 4);
    }
    TEST_CASE("String can be copied")
    {
        UTF8String str{std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        }};
        str.append(' ');
        UTF8String str2(str);

        REQUIRE(str.get_byte_count() == 12);
        REQUIRE(str.get_point_count() == 5);

        REQUIRE(str2.get_byte_count() == 12);
        REQUIRE(str2.get_point_count() == 5);

        REQUIRE(str == str2);

        UTF8String str3 = str;
        REQUIRE(str3 == str);
    }
    TEST_CASE("Non-empty string can be assigned")
    {
        UTF8String str{std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        }};
        str.append(' ');

        UTF8String str2{std::vector<CodePoint>{
            0x1F614,
            0x010D,
            static_cast<CodePoint>('f')}};
        str2.append('a');

        str = str2;

        REQUIRE(str2.get_byte_count() == 8);
        REQUIRE(str2.get_point_count() == 4);

        REQUIRE(str.get_byte_count() == 8);
        REQUIRE(str.get_point_count() == 4);

        REQUIRE(str == str2);
    }
    TEST_CASE("Empty string can be assigned")
    {
        UTF8String str2{std::vector<CodePoint>{
            0x1F614,
            0x010D,
            static_cast<CodePoint>('f')}};
        str2.append('a');

        UTF8String str;
        str = str2;

        REQUIRE(str2.get_byte_count() == 8);
        REQUIRE(str2.get_point_count() == 4);

        REQUIRE(str.get_byte_count() == 8);
        REQUIRE(str.get_point_count() == 4);

        REQUIRE(str == str2);
    }
    TEST_CASE("Self assignment")
    {
        UTF8String str1{std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        }};
        str1 = str1;

        REQUIRE(str1.get_byte_count() == 11);
        REQUIRE(str1.get_point_count() == 4);

        UTF8String str2{"string"};
        str2 = str2;

        REQUIRE(str2 == UTF8String{"string"});
    }
    TEST_CASE("Deep copy is created when copying a string")
    {
        UTF8String s1{"aaa"};
        UTF8String s2{s1};
        UTF8String s3{"ccc"};
        s3 = s1;

        const UTF8String *const p1 = &s1;
        const UTF8String *const p2 = &s2;
        const UTF8String *const p3 = &s3;

        REQUIRE_NE(memcmp(p1, p2, sizeof(UTF8String)), 0);
        REQUIRE_NE(memcmp(p1, p3, sizeof(UTF8String)), 0);
        REQUIRE_NE(memcmp(p2, p3, sizeof(UTF8String)), 0);
    }
    TEST_CASE("String equality 1")
    {
        UTF8String s1{"aaaaaa"};
        UTF8String s2{"aaaaaa"};
        UTF8String s3{"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};

        REQUIRE(s1 == s1);
        REQUIRE(s1 == s2);
        REQUIRE(not(s1 == s3));
        REQUIRE(not(s3 == s1));
    }
    TEST_CASE("String equality 2")
    {
        UTF8String s1{"aaaaaa"};
        UTF8String s2{"bbbbbb"};
        UTF8String s3{"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};

        REQUIRE(s1 != s2);
        REQUIRE(s3 != s1);
    }
    TEST_CASE("String equality 3")
    {
        UTF8String s1{
            std::vector<CodePoint>{
                0x1F601, // 😁
                0x1F614, // 😔
                static_cast<CodePoint>('0'),
                0x010D // č
            }};

        UTF8String s2{
            std::vector<CodePoint>{
                0x1F602, // succ 😁
                0x1F614, // 😔
                static_cast<CodePoint>('0'),
                0x010D // č
            }};

        REQUIRE(s1 != s2);
    }
    TEST_CASE("String equality 4")
    {
        UTF8String s1{
            std::vector<CodePoint>{
                0x1F601, // 😁
                0x1F614, // 😔
                static_cast<CodePoint>('0'),
                0x010D // č
            }};

        UTF8String s2{
            std::vector<CodePoint>{
                0x1F601, // 😁
                0x1F614, // 😔
                static_cast<CodePoint>('0'),
                0x010D // č
            }};

        REQUIRE(s1 == s2);
    }
    TEST_CASE("Byte indexing")
    {
        const UTF8String str{std::vector<CodePoint>{
            static_cast<CodePoint>('A'),
            static_cast<CodePoint>('h'),
            static_cast<CodePoint>('o'),
            static_cast<CodePoint>('j'),
            static_cast<CodePoint>(' '),
            0x1F601, // 😁
            static_cast<CodePoint>('.'),
        }};

        REQUIRE(str.get_byte_count() == 10);
        REQUIRE(str.get_point_count() == 7);

        REQUIRE(str[0].value() == 'A');
        REQUIRE(str[1].value() == 'h');
        REQUIRE(str[2].value() == 'o');
        REQUIRE(str[3].value() == 'j');
        REQUIRE(str[4].value() == ' ');
        REQUIRE(str[5].value() == 0xF0);
        REQUIRE(str[6].value() == 0x9F);
        REQUIRE(str[7].value() == 0x98);
        REQUIRE(str[8].value() == 0x81);
        REQUIRE(str[9].value() == '.');
    }
    TEST_CASE("Code point indexing")
    {
        UTF8String string;
        string.append('A');
        string.append('h');
        string.append('o');
        string.append('j');
        string.append(' ');
        string.append(static_cast<CodePoint>(0x1F601));
        string.append('.');

        REQUIRE(string.get_byte_count() == 10);
        REQUIRE(string.get_point_count() == 7);

        const UTF8String &str = string;
        REQUIRE(str.nth_code_point(0).value() == 'A');
        REQUIRE(str.nth_code_point(1).value() == 'h');
        REQUIRE(str.nth_code_point(2).value() == 'o');
        REQUIRE(str.nth_code_point(3).value() == 'j');
        REQUIRE(str.nth_code_point(4).value() == ' ');
        REQUIRE(str.nth_code_point(5).value() == 0x1F601);
        REQUIRE(str.nth_code_point(6).value() == '.');
    }
    TEST_CASE("Invalid byte index")
    {
        UTF8String str{"foo"};
        REQUIRE(!str[4].has_value());
    }
    TEST_CASE("Invalid code point index")
    {
        UTF8String str;
        str.append('f');
        str.append(static_cast<CodePoint>(0x1F601));
        str.append('o');
        REQUIRE(!str.nth_code_point(4).has_value());
    }
    TEST_CASE("Append char")
    {
        UTF8String str{std::vector<CodePoint>{
            static_cast<CodePoint>('a'),
            0x1F601 // 😁
        }};
        str.append('a');
        REQUIRE(str.get_byte_count() == 6);
        REQUIRE(str.get_point_count() == 3);

        REQUIRE(str[5].value() == 'a');
    }
    TEST_CASE("Append char stress test")
    {
        UTF8String str;

        size_t count = 10'000;
        for (size_t i = 0; i < count; i++)
        {
            str.append('a');
        }
        REQUIRE(str.get_byte_count() == count);
        REQUIRE(str.get_point_count() == count);

        REQUIRE(str == UTF8String{std::vector<CodePoint>(count, static_cast<CodePoint>('a'))});
    }
    TEST_CASE("Append code point 1")
    {
        UTF8String str{std::vector<CodePoint>{
            static_cast<CodePoint>('a'),
            0x1F601 // 😁
        }};
        str.append(static_cast<CodePoint>('b'));
        str.append(static_cast<CodePoint>(0x1F602)); // 😂

        REQUIRE(str.get_byte_count() == 10);
        REQUIRE(str.get_point_count() == 4);

        REQUIRE(str[0].value() == 'a');
        REQUIRE(str[5].value() == 'b');
        REQUIRE(str[6].value() == 0xF0);
        REQUIRE(str[7].value() == 0x9F);
        REQUIRE(str[8].value() == 0x98);
        REQUIRE(str[9].value() == 0x82);
    }
    TEST_CASE("Append code point 2")
    {
        UTF8String str;
        str.append(static_cast<CodePoint>('a'));
        str.append(static_cast<CodePoint>(0x00A3));  // £
        str.append(static_cast<CodePoint>(0x20AC));  // €
        str.append(static_cast<CodePoint>(0x10348)); // 𐍈

        REQUIRE(str.get_byte_count() == 10);
        REQUIRE(str.get_point_count() == 4);
    }
    TEST_CASE("Append code point stress test")
    {
        UTF8String str;

        size_t count = 1'000;
        for (size_t i = 0; i < count; i++)
        {
            str.append(static_cast<CodePoint>(0x20AC)); // €
        }
        REQUIRE(str.get_byte_count() == count * 3);
        REQUIRE(str.get_point_count() == count);

        REQUIRE(str == UTF8String{std::vector<CodePoint>(count, 0x20AC)});
    }
    TEST_CASE("Join strings 1")
    {
        UTF8String str{"Hello"};
        UTF8String str2{" world"};

        REQUIRE((str + str2) == UTF8String{"Hello world"});
        REQUIRE(str == UTF8String{"Hello"});
        REQUIRE(str2 == UTF8String{" world"});
    }
    TEST_CASE("Join strings 2")
    {
        UTF8String str{"Hello"};

        REQUIRE((str + UTF8String{}) == UTF8String{"Hello"});
        REQUIRE((UTF8String{} + str) == UTF8String{"Hello"});
    }
    TEST_CASE("Join strings 3")
    {
        UTF8String str{"Hello"};
        UTF8String str2{std::vector<CodePoint>{
            0x20AC, // €
            static_cast<CodePoint>('f')}};

        auto res1 = str + str2;
        REQUIRE(res1.get_byte_count() == 9);
        REQUIRE(res1.get_point_count() == 7);

        auto res2 = str2 + str;
        REQUIRE(res2.get_byte_count() == 9);
        REQUIRE(res2.get_point_count() == 7);
    }
    TEST_CASE("Append strings 1")
    {
        UTF8String str{"Hello"};
        UTF8String str2{", "};
        UTF8String str3{"World"};
        UTF8String str4{"!"};

        str += str2;
        str += str3;
        str += str4;

        REQUIRE(str == UTF8String{"Hello, World!"});
    }
    TEST_CASE("Append strings 2")
    {
        UTF8String str{"Hello"};
        UTF8String str2{std::vector<CodePoint>{
            0x20AC, // €
            static_cast<CodePoint>('f')}};

        str += str2;
        REQUIRE(str.get_byte_count() == 9);
        REQUIRE(str.get_point_count() == 7);
    }
    TEST_CASE("Test cast")
    {
        UTF8String str{"Ahoj"};

        std::string str2 = static_cast<std::string>(str);
        REQUIRE(str2 == "Ahoj");
    }
}

template <typename Item, typename Iterator>
std::vector<Item> collect(Iterator iterator)
{
    std::vector<Item> items;
    for (const auto &item : iterator)
    {
        items.push_back(item);
    }
    return items;
}

// This function is used to avoid problems with SFINAE in GCC 11.
template <typename T>
void check_vectors(std::vector<T> a, std::vector<T> b)
{
    REQUIRE_EQ(a.size(), b.size());
    for (size_t i = 0; i < a.size(); i++)
    {
        REQUIRE_EQ(a[i], b[i]);
    }
}

// New string tests with move semantics and iterator
TEST_SUITE("UTF8String new")
{
    TEST_CASE("Move string")
    {
        auto fn = [](UTF8String string)
        {
            REQUIRE(string.get_byte_count() == 4);
            REQUIRE(string.get_point_count() == 4);
            REQUIRE(string[0].value() == 'a');
            REQUIRE(string[1].value() == 'h');
            REQUIRE(string[2].value() == 'o');
            REQUIRE(string[3].value() == 'j');
        };

        UTF8String str{"ahoj"};
        fn(std::move(str));
        REQUIRE(str.get_point_count() == 0);
    }

    TEST_CASE("Move assign")
    {
        UTF8String str{"ahoj"};
        str = UTF8String{"cau"};

        REQUIRE(str.get_point_count() == 3);
    }

    TEST_CASE("Iterate bytes empty")
    {
        UTF8String str;
        REQUIRE(collect<uint8_t>(str.bytes()).empty());

        auto it = str.bytes();
        REQUIRE(!(it.begin() != it.end()));
    }

    TEST_CASE("Const string can be byte-iterated")
    {
        const UTF8String str{"ahoj"};
        auto iterator = str.bytes().begin();
        ++iterator;
    }

    TEST_CASE("Iterate bytes")
    {
        UTF8String str{std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        }};
        check_vectors(collect<uint8_t>(str.bytes()), std::vector<uint8_t>{
                                                         0xF0, 0x9F, 0x98, 0x81,
                                                         0xF0, 0x9F, 0x98, 0x94,
                                                         0x30,
                                                         0xC4, 0x8D});
    }

    TEST_CASE("Iterate bytes concurrently")
    {
        UTF8String str{"ahoj"};

        auto byte_iter = str.bytes();
        auto it1 = byte_iter.begin();
        auto it2 = byte_iter.begin();

        REQUIRE(*it1 == 'a');
        REQUIRE(*it2 == 'a');
        ++it2;
        REQUIRE(*it1 == 'a');
        REQUIRE(*it2 == 'h');
        ++it2;
        ++it1;
        REQUIRE(*it1 == 'h');
        REQUIRE(*it2 == 'o');
    }

    TEST_CASE("Random access byte iterators")
    {
        UTF8String str{"I Love Haskell"};

        auto it = str.bytes().begin();

        it += 7;
        REQUIRE_EQ(*it, 'H');
        it -= 3;
        REQUIRE_EQ(*it, 'v');

        auto it2 = it + 6;
        REQUIRE_EQ(*it2, 'k');

        auto it3 = it2 - 2;
        REQUIRE_EQ(*it3, 'a');
    }

    TEST_CASE("Iterate codepoints empty")
    {
        UTF8String str;
        REQUIRE(collect<CodePoint>(str.codepoints()).empty());

        auto it = str.codepoints();
        REQUIRE(!(it.begin() != it.end()));
    }

    TEST_CASE("Const string can be codepoint-iterated")
    {
        const UTF8String str{"ahoj"};
        auto iterator = str.codepoints().begin();
        ++iterator;
    }

    TEST_CASE("Bidirectional codepoint iterators")
    {
        UTF8String str{std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        }};

        auto it = str.codepoints().begin();

        ++it;
        REQUIRE_EQ(*it, 0x1F614);
        ++it;
        REQUIRE_EQ(*it, static_cast<CodePoint>('0'));
        --it;
        REQUIRE_EQ(*it, 0x1F614);
        --it;
        REQUIRE_EQ(*it, 0x1F601);
    }

    TEST_CASE("Iterate codepoints 1")
    {
        UTF8String str{std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        }};
        check_vectors(collect<CodePoint>(str.codepoints()), std::vector<CodePoint>{
                                                                0x1F601, // 😁
                                                                0x1F614, // 😔
                                                                static_cast<CodePoint>('0'),
                                                                0x010D // č
                                                            });
    }

    TEST_CASE("Iterate codepoints 2")
    {
        std::vector<uint8_t> utf8input{
            0xF0, 0x9F, 0x98, 0x81,
            0xF0, 0x9F, 0x98, 0x94,
            0x30,
            0xC4, 0x8D};
        UTF8String str{utf8input};
        check_vectors(collect<CodePoint>(str.codepoints()), std::vector<CodePoint>{
                                                                0x1F601, // 😁
                                                                0x1F614, // 😔
                                                                static_cast<CodePoint>('0'),
                                                                0x010D // č
                                                            });
    }

    TEST_CASE("Iterate codepoints concurrently")
    {
        UTF8String str{std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        }};

        auto codepoint_iter = str.codepoints();
        auto it1 = codepoint_iter.begin();
        auto it2 = codepoint_iter.begin();

        REQUIRE(*it1 == 0x1F601);
        REQUIRE(*it2 == 0x1F601);
        ++it2;
        REQUIRE(*it1 == 0x1F601);
        REQUIRE(*it2 == 0x1F614);
        ++it2;
        ++it1;
        REQUIRE(*it1 == 0x1F614);
        REQUIRE(*it2 == static_cast<CodePoint>('0'));
    }
}

// Tree tests
TEST_SUITE("Tree")
{
    TEST_CASE("Create tree")
    {
        /**
         * tree:
         *     0
         *    / \
         *   1   2
         */
        auto tree = std::make_unique<Tree>(0);
        tree->set_left_child(std::make_unique<Tree>(1));
        tree->set_right_child(std::make_unique<Tree>(2));

        REQUIRE(tree->get_value().value == 0);
        REQUIRE(tree->get_left_child()->get_value().value == 1);
        REQUIRE(tree->get_right_child()->get_value().value == 2);
    }

    TEST_CASE("Reassign child")
    {
        /**
         * tree:
         *     0
         *    / \
         *   1   2
         */
        auto tree = std::make_unique<Tree>(0);
        auto previous = tree->set_left_child(std::make_unique<Tree>(1));
        tree->set_right_child(std::make_unique<Tree>(2));

        REQUIRE(previous.get() == nullptr);

        /**
         * tree:
         *     0
         *    / \
         *   3   2
         *
         * previous:
         *     1
         */
        previous = tree->set_left_child(std::make_unique<Tree>(3));
        REQUIRE(previous->get_value().value == 1);
        REQUIRE(!previous->has_parent());

        REQUIRE(tree->get_left_child()->get_value().value == 3);
        REQUIRE(tree->get_left_child()->get_parent() == tree.get());
    }

    TEST_CASE("Parent links")
    {
        /**
         * tree:
         *     0
         *    / \
         *   1   2
         */
        auto tree = std::make_unique<Tree>(0);
        tree->set_left_child(std::make_unique<Tree>(1));
        tree->set_right_child(std::make_unique<Tree>(2));

        REQUIRE(!tree->has_parent());
        REQUIRE(tree->get_left_child()->get_parent() == tree.get());
        REQUIRE(tree->get_right_child()->get_parent() == tree.get());
    }

    TEST_CASE("Set empty child")
    {
        auto tree = std::make_unique<Tree>(0);
        tree->set_left_child(std::make_unique<Tree>(1));
        tree->set_left_child(std::unique_ptr<Tree>{});
        REQUIRE(tree->get_left_child() == nullptr);
    }

    TEST_CASE("Take children")
    {
        /**
         * tree:
         *     0
         *    / \
         *   1   2
         */
        auto tree = std::make_unique<Tree>(0);
        tree->set_left_child(std::make_unique<Tree>(1));
        tree->set_right_child(std::make_unique<Tree>(2));

        /**
         * tree:
         *     0
         *      \
         *       2
         *
         * left:
         *     1
         */
        auto left = tree->take_left_child();
        REQUIRE(!left->has_parent());

        REQUIRE(tree->get_left_child() == nullptr);
    }

    TEST_CASE("Take children with take_child")
    {
        auto tree = std::make_unique<Tree>(0);
        tree->set_left_child(std::make_unique<Tree>(1));
        auto child = tree->take_child(*tree->get_left_child());

        REQUIRE(!child->has_parent());
        REQUIRE(child->get_value().value == 1);
        REQUIRE(tree->get_left_child() == nullptr);
    }

    TEST_CASE("Take empty children")
    {
        auto tree = std::make_unique<Tree>(0);
        REQUIRE(!tree->take_left_child());
        REQUIRE(!tree->take_right_child());
    }

    TEST_CASE("Take invalid children")
    {
        auto tree = std::make_unique<Tree>(0);
        auto tree2 = std::make_unique<Tree>(0);

        REQUIRE_THROWS(tree->take_child(*tree2));
    }

    TEST_CASE("Swap children")
    {
        /**
         * tree:
         *     0
         *    / \
         *   1   2
         */
        auto tree = std::make_unique<Tree>(0);
        tree->set_left_child(std::make_unique<Tree>(1));
        tree->set_right_child(std::make_unique<Tree>(2));

        /**
         * tree:
         *     0
         *    / \
         *   2   1
         */
        tree->swap_children();

        REQUIRE(tree->get_left_child()->get_value().value == 2);
        REQUIRE(tree->get_right_child()->get_value().value == 1);
    }

    TEST_CASE("Rebalance tree")
    {
        /*
         * tree:
         *       0
         *      /
         *     1  <- parent
         *    /
         *   2
         *  /
         * 3   <- last
         */
        auto tree = std::make_unique<Tree>(0);
        Tree *last = tree.get();
        for (int i = 1; i < 4; i++)
        {
            last->set_left_child(std::make_unique<Tree>(i));
            last = last->get_left_child();
        }
        // `last` now holds the last added child
        auto parent = last->get_parent()->get_parent();

        /*
         * tree:
         *      0
         *     / \
         *    1   2
         *       /
         *      3
         */
        auto subtree = parent->take_left_child();
        tree->set_right_child(std::move(subtree));
    }

    std::unique_ptr<Tree> christmas_tree()
    {
        /*
         * tree:
         *               1
         *              / \
         *             /   \
         *            /     \
         *           2       5
         *          / \     / \
         *         3   4   6   7
         *        /             \
         *       8               9
         */
        auto tree = std::make_unique<Tree>(1);
        tree->set_left_child(std::make_unique<Tree>(2));
        tree->get_left_child()->set_left_child(std::make_unique<Tree>(3));
        tree->get_left_child()->set_right_child(std::make_unique<Tree>(4));
        tree->get_left_child()->get_left_child()->set_left_child(std::make_unique<Tree>(8));

        tree->set_right_child(std::make_unique<Tree>(5));
        tree->get_right_child()->set_left_child(std::make_unique<Tree>(6));
        tree->get_right_child()->set_right_child(std::make_unique<Tree>(7));
        tree->get_right_child()->get_right_child()->set_right_child(std::make_unique<Tree>(9));
        return tree;
    }

    TEST_CASE("Get root")
    {
        auto tree = christmas_tree();

        REQUIRE(tree->get_root() == tree.get());
        REQUIRE(tree->get_left_child()->get_right_child()->get_root() == tree.get());
        REQUIRE(tree->get_left_child()->get_right_child()->get_root() == tree.get());
        REQUIRE(tree->get_right_child()->get_right_child()->get_right_child()->get_root() == tree.get());
    }

    TEST_CASE("Is same tree")
    {
        auto t1 = christmas_tree();
        auto t2 = christmas_tree();

        REQUIRE(t1->is_same_tree_as(t1.get()));
        REQUIRE(t1->get_right_child()->is_same_tree_as(t1.get()));
        REQUIRE(t1->get_right_child()->get_left_child()->is_same_tree_as(t1.get()));
        REQUIRE(t1->is_same_tree_as(t1->get_right_child()->get_left_child()));

        REQUIRE(!t1->is_same_tree_as(t2.get()));
        REQUIRE(!t1->get_left_child()->get_right_child()->is_same_tree_as(t2.get()));
        REQUIRE(!t2->is_same_tree_as(t1->get_left_child()->get_right_child()));
        REQUIRE(!t2->get_right_child()->get_left_child()->is_same_tree_as(t1->get_left_child()->get_right_child()));
    }

    TEST_CASE("Shared values")
    {
        /**
         * tree:
         *        0 (V0)
         *      /   \
         *   1 (V0)   2 (V1)
         */

        auto value = std::make_shared<BigData>(0);
        auto tree = std::make_unique<Tree>(value);
        tree->set_left_child(std::make_unique<Tree>(value));
        tree->set_right_child(std::make_unique<Tree>(1));

        tree->get_left_child()->get_value().value = 5;
        REQUIRE(tree->get_value().value == 5);
        REQUIRE(tree->get_right_child()->get_value().value == 1);
    }

    TEST_CASE("Replace shared values")
    {
        /*
         * tree:
         *               1
         *              / \
         *             /   \
         *            /     \
         *           2       5
         *          / \     / \
         *         3   4   6   7
         *        /             \
         *       8               9
         */
        auto tree = christmas_tree();
        auto n2 = tree->get_left_child();

        // Set shared value of left subtree to 50
        n2->replace_value(std::make_shared<BigData>(50));
        REQUIRE(n2->get_value().value == 50);
        REQUIRE(n2->get_left_child()->get_value().value == 50);
        REQUIRE(n2->get_left_child()->get_left_child()->get_value().value == 50);
        REQUIRE(n2->get_right_child()->get_value().value == 50);

        n2->get_value().value = 51;
        REQUIRE(n2->get_value().value == 51);
        REQUIRE(n2->get_left_child()->get_value().value == 51);
        REQUIRE(n2->get_left_child()->get_left_child()->get_value().value == 51);
        REQUIRE(n2->get_right_child()->get_value().value == 51);

        REQUIRE(tree->get_value().value == 1);
    }

    TEST_CASE("Inorder traversal (bonus)")
    {
        auto tree = christmas_tree();

        std::vector<int> values;
        for (const auto &node : *tree)
        {
            values.push_back(node.get_value().value);
        }

        REQUIRE(values == std::vector<int>{8, 3, 2, 4, 1, 6, 5, 7, 9});
    }
}
