// This tells doctest to provide a main() function
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "tasks.h"

#include <sstream>
#include <vector>

// Comment out tests and functions that use code that you haven't implemented yet

static std::string print(const Value &value)
{
    std::stringstream ss;
    PrintVisitor visitor{ss};
    value.accept(visitor);
    return ss.str();
}

TEST_SUITE("Integer")
{
    TEST_CASE("Create integer")
    {
        const Integer value{1};
        REQUIRE(value.get_value() == 1);
    }

    TEST_CASE("Index integer")
    {
        const Integer value{1};
        REQUIRE_THROWS(value[0]);
        REQUIRE_THROWS(value["a"]);
    }
}

TEST_SUITE("Null")
{
    TEST_CASE("Index null")
    {
        const Null null;
        REQUIRE_THROWS(null[0]);
        REQUIRE_THROWS(null["a"]);
    }
}

TEST_SUITE("Array")
{
    TEST_CASE("Create array")
    {
        const Array value{{new Integer{1}, new Integer{2}}};
        REQUIRE(value.size() == 2);
    }

    TEST_CASE("Index array")
    {
        const Array value{{new Integer{1}, new Integer{2}, new Null{}}};
        REQUIRE(dynamic_cast<const Integer *>(value[0])->get_value() == 1);
        REQUIRE(dynamic_cast<const Integer *>(value[1])->get_value() == 2);
        REQUIRE(dynamic_cast<const Null *>(value[2]));
        REQUIRE(value[3] == nullptr);
    }

    TEST_CASE("Add item to array")
    {
        Array value{{new Integer{1}, new Integer{2}}};
        value.append(new Integer{3});
        REQUIRE(value.size() == 3);
        REQUIRE(dynamic_cast<const Integer *>(value[2])->get_value() == 3);
    }

    TEST_CASE("Add items to array")
    {
        Array value{{new Integer{1}, new Integer{2}}};
        for (int i = 0; i < 100; i++)
        {
            value.append(new Integer{i});
        }
        REQUIRE(value.size() == 102);
    }
}

TEST_SUITE("Object")
{
    TEST_CASE("Create object")
    {
        auto *value = new Array{{new Integer{1}, new Integer{2}}};
        const Object object{{{"a", value},
                             {"b", new Integer{2}},
                             {"c", new Null{}}}};
        REQUIRE(object.size() == 3);
        REQUIRE(object.keys() == std::vector<std::string>{"a", "b", "c"});
    }

    TEST_CASE("Index object")
    {
        const Object object{{
            {"x", new Integer{1}},
            {"y", new Integer{3}},
            {"b", new Integer{2}},
        }};
        REQUIRE(dynamic_cast<const Integer *>(object["b"])->get_value() == 2);
        REQUIRE(object["i"] == nullptr);
    }

    TEST_CASE("Use owned string as a key")
    {
        Object object{};
        {
            std::string key = "foo";
            object.insert(key, new Integer{1});
        }
        REQUIRE(dynamic_cast<const Integer *>(object["foo"])->get_value() == 1);
    }

    TEST_CASE("Add item to object")
    {
        Object object{{
            {"a", new Integer{1}},
            {"b", new Integer{3}},
            {"c", new Integer{2}},
        }};
        object.insert("d", new Integer{10});
        object.insert("e", new Array{{new Integer{11}, new Integer{12}}});
        REQUIRE(object.size() == 5);
        REQUIRE(dynamic_cast<const Array *>(object["e"])->size() == 2);
    }

    TEST_CASE("Overwrite item in object")
    {
        Object object{{
            {"a", new Integer{1}},
            {"b", new Integer{3}},
            {"c", new Integer{2}},
        }};
        object.insert("a", new Integer{10});
        REQUIRE(object.size() == 3);
        REQUIRE(dynamic_cast<const Integer *>(object["a"])->get_value() == 10);
    }
}

TEST_SUITE("Indexing")
{
    TEST_CASE("Index value with a number")
    {
        const Value *value = new Array{{new Integer{1}, new Integer{1}}};
        REQUIRE(dynamic_cast<const Integer *>((*value)[0])->get_value() == 1);
        delete value;
    }
    TEST_CASE("Index object")
    {
        const Value *object = new Object{{
            {"x", new Integer{1}},
            {"y", new Integer{3}},
            {"b", new Integer{2}},
        }};
        REQUIRE(dynamic_cast<const Integer *>((*object)["b"])->get_value() == 2);
        delete object;
    }
}

TEST_SUITE("Clone")
{
    TEST_CASE("Integer")
    {
        const auto *a = new Integer{5};
        auto *b = a->clone();
        REQUIRE(a != b);
        REQUIRE(b->get_value() == 5);

        delete a;
        delete b;
    }
    TEST_CASE("Null")
    {
        const auto *a = new Null();
        auto *b = a->clone();
        REQUIRE(a != b);

        delete a;
        delete b;
    }
    TEST_CASE("Array")
    {
        auto *obj = new Object{{
            {"a", new Integer{1}},
            {"b", new Integer{3}},
            {"c", new Integer{2}},
        }};
        auto *value = new Array{{new Integer{1},
                                 new Integer{1},
                                 new Array{{new Integer{10}, new Integer{11}}},
                                 new Integer{4},
                                 obj}};
        const auto *cloned = value->clone();
        REQUIRE(cloned != value);
        REQUIRE(cloned->size() == 5);
        REQUIRE(dynamic_cast<const Integer *>((*cloned)[0])->get_value() == 1);

        value->append(new Integer{2});
        REQUIRE(cloned->size() == 5);

        obj->insert("x", new Integer{4});

        auto *new_obj = (*cloned)[4];
        REQUIRE((*new_obj)["x"] == nullptr);

        delete cloned;
        delete value;
    }
    TEST_CASE("Array deep copy")
    {
        const auto *value = new Array{{
            new Integer{1},
        }};
        auto *cloned = value->clone();
        REQUIRE((*value)[0] != (*cloned)[1]);

        delete value;
        delete cloned;
    }
    TEST_CASE("Object")
    {
        const auto *value = new Object{{
            {"a", new Integer{1}},
            {"b", new Integer{3}},
            {"c", new Integer{2}},
        }};
        auto *cloned = value->clone();
        REQUIRE(cloned != value);
        REQUIRE(cloned->size() == 3);

        cloned->insert("x", new Integer{5});
        REQUIRE(value->size() == 3);

        delete value;
        delete cloned;
    }
    TEST_CASE("Object deep copy")
    {
        const auto *value = new Object{{
            {"foo", new Integer{3}},
        }};
        const auto *cloned = value->clone();
        REQUIRE((*value)["foo"] != (*cloned)["foo"]);

        delete value;
        delete cloned;
    }
    TEST_CASE("Value")
    {
        const Value *value = new Object{{
            {"a", new Integer{1}},
            {"b", new Integer{3}},
            {"c", new Integer{2}},
        }};
        auto *cloned = dynamic_cast<Object *>(value->clone());
        REQUIRE(cloned != value);
        REQUIRE(cloned->size() == 3);

        cloned->insert("x", new Integer{5});
        REQUIRE(dynamic_cast<const Object *>(value)->size() == 3);

        delete value;
        delete cloned;
    }
}

TEST_SUITE("Print")
{
    TEST_CASE("Integer")
    {
        Integer integer{5};
        REQUIRE(print(integer) == "5");
    }

    TEST_CASE("Empty array")
    {
        REQUIRE(print(Array{}) == "[]");
    }

    TEST_CASE("Array")
    {
        auto *obj = new Object{{{"a", new Integer{5}},
                                {"b", new Array{{new Integer{1}, new Integer{2}}}}}};
        const Array value{{
            new Integer{1},
            new Integer{2},
            obj,
            new Array{{new Integer{10}, new Integer{11}}},
            new Integer{4},
            new Null{},
        }};
        REQUIRE(print(value) == "[1, 2, {a: 5, b: [1, 2]}, [10, 11], 4, null]");
    }

    TEST_CASE("Empty object")
    {
        REQUIRE(print(Object{}) == "{}");
    }

    TEST_CASE("Object")
    {
        auto *array = new Array{{
            new Integer{1},
            new Integer{2},
            new Array{{new Integer{10}, new Integer{11}}},
            new Integer{4},
        }};
        const Object value{{
            {"a", new Integer{5}},
            {"b", new Array{{new Integer{1}, new Integer{3}}}},
            {"c", array},
            {"d", new Object{{{"a", new Integer{5}},
                              {"b", new Integer{6}}}}},
            {"e", new Null()},
        }};
        REQUIRE(print(value) == "{a: 5, b: [1, 3], c: [1, 2, [10, 11], 4], d: {a: 5, b: 6}, e: null}");
    }

    TEST_CASE("Print value")
    {
        Value *value = new Array{{new Integer{1}, new Integer{2}}};
        REQUIRE(print(*value) == "[1, 2]");
        delete value;
    }
}

TEST_SUITE("Remove null")
{
    TEST_CASE("Remove null")
    {
        auto *object = new Object{{
            {"a", new Integer{1}},
            {"b", new Null{}},
            {"c", new Integer{2}},
            {"d", new Object{{{"a", new Null{}},
                              {"b", new Array(
                                        {new Integer{1}, new Null{}, new Integer{3}, new Integer{2}, new Null{}})}}}},
        }};

        RemoveNullVisitor v;
        object->accept(v);

        REQUIRE(object->keys() == std::vector<std::string>{"a", "c", "d"});

        auto *d = dynamic_cast<Object *>(object->operator[]("d"));
        REQUIRE_NE(d, nullptr);

        REQUIRE(d->keys() == std::vector<std::string>{"b"});

        auto *arr = dynamic_cast<Array *>(d->operator[]("b"));
        REQUIRE_NE(arr, nullptr);
        REQUIRE_EQ(arr->size(), 3);
        REQUIRE_NE(dynamic_cast<Integer *>(arr->operator[](1)), nullptr);

        delete object;
    }
}
