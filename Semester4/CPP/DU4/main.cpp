#include "tasks.h"
#include <iostream>

void print(std::string str)
{
    std::cout << str << std::endl;
}

void print(int value)
{
    std::cout << value << std::endl;
}

int main()
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

    auto d = dynamic_cast<Object *>(object->operator[]("d"));

    auto keys = d->keys();

    for (auto key : keys)
    {
        print(key);
    }

    auto *arr = dynamic_cast<Array *>(d->operator[]("b"));

    print(arr->size());

    delete object;
}
