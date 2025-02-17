#include "tasks.h"
#include <sstream>

static std::pair<std::string, Value> kv(std::string key, Value value)
{
    return std::make_pair(key, std::move(value));
}

template <size_t N>
static Value object(std::array<std::pair<std::string, Value>, N> expected = {})
{
    Object object{};
    for (auto &item : expected)
    {
        object.items[item.first] = std::move(item.second);
    }
    return object;
}

int main()
{

    std::array values{
        kv("yes", Boolean{true}),
        kv("no", Boolean{false}),
        kv("null", Null{}),
        kv("number", Number{666}),
        kv("string", String{"Inside my shell I wait and bleed"})};

    auto o = object(values);

    auto ser = serialize(o);

    for (auto it = ser.begin(); it != ser.end(); ++it)
    {
        std::cout << static_cast<int>(*it) << std::endl;
    }

    Value des = deserialize(ser);

    std::cout << des << std::endl;

    return 0;
}
