#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "tuple.hpp"

template <typename T>
concept HasSize = requires(T a) {
    {
        a.size()
    };
};

template <typename T>
class UniquePtr
{
    T *ptr = nullptr;

public:
    UniquePtr(T *t)
    {
        this->ptr = t;
    }

    ~UniquePtr()
    {
        delete this->ptr;
    }

    UniquePtr(const UniquePtr &) = delete;

    UniquePtr(UniquePtr &&other) : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    T &operator*()
    {
        return *this->ptr;
    }

    const T &operator*() const
    {
        return *this->ptr;
    }

    int getNum();
};

template <typename T>
int UniquePtr<T>::getNum()
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return ptr->size();
    }
    else
    {
        return *ptr;
    }
}

template <HasSize T>
int getNum2(UniquePtr<T> &ptr)
{
    return (*ptr).size();
}

template <typename T>
int getNum2(UniquePtr<T> &ptr)
{
    return *ptr;
}

template <typename T>
UniquePtr<T> makeUnique(T *t)
{
    return UniquePtr{t};
}

struct S
{
    ~S()
    {
        std::cout << "~S" << std::endl;
    }
};

auto fold(auto begin, auto end, auto initialValue, const auto &fn)
{
    for (; begin != end; ++begin)
    {
        fn(initialValue, *begin);
    }

    return initialValue;
}

// template <typename T, typename Callable>
/* kdyby nebylo auto, tak třeba: decltype(Callable{}(T{}))*/ auto apply(const /*T*/ auto &t, const /*Callable*/ auto &c) // -> decltype(c(t)) // tady už kompilátor zná parametry, takže nemusím resolvit typy
{
    return c(t);
}

// umi resolvit i reference
// Defaultně auto neresolví reference
decltype(auto) deref(auto fn)
{
    return fn();
}

template <typename T, typename... Ts>
void print(const T &t, const Ts &...ts)
{
    std::cout << t << ", ";
    if constexpr (sizeof...(ts) > 0)
    {
        print(ts...);
    }
    else
    {
        std::endl(std::cout);
    }
}

template <typename... Ts>
auto sum(Ts... ts)
{
    return (ts + ...);
}

template <typename T, std::enable_if_t<std::is_same_v<T, int>, bool> = true>
int loadItem(const std::string &str)
{
    return std::stoi(str);
}

template <typename T, std::enable_if_t<std::is_same_v<T, double>, bool> = true>
double loadItem(const std::string &str)
{
    return std::stod(str);
}

template <typename T, std::enable_if_t<std::is_same_v<T, std::string>, bool> = true>
std::string loadItem(const std::string &str)
{
    return str;
}

template <typename T, std::enable_if_t<std::is_same_v<T, bool>, bool> = true>
bool loadItem(const std::string &str)
{
    return str == "true";
}

struct Struct
{
    int x;
};

template <typename T, std::enable_if_t<std::is_same_v<T, Struct>, bool> = true>
Struct loadItem(const std::string &str)
{
    return {
        std::stoi(str) * 5};
}

template <int index>
void parseItem(auto &, std::stringstream &) {}

template <int index, typename T, typename... Ts>
void parseItem(auto &tuple, std::stringstream &ss)
{
    std::string item;
    std::getline(ss, item, ',');

    std::get<index>(tuple) = loadItem<T>(item);

    parseItem<index + 1, Ts...>(tuple, ss);
}

template <template <typename...> typename TupleType, typename... Ts>
TupleType<Ts...> parse(const std::string &str)
{
    TupleType<Ts...> result;

    std::stringstream ss{str};

    parseItem<0, Ts...>(result, ss);

    return result;
}

int main()
{
    print(1, 2.3, "hello", sum(1, 2, 3));

    std::string line = "2,string,2.3,true";

    auto parsed = parse<yy::Tuple, Struct, std::string, double, bool>(line);

    std::cout << " { " << std::get<0>(parsed).x << ", " << std::get<1>(parsed) << ", " << std::get<2>(parsed) << ", " << std::boolalpha << std::get<3>(parsed) << " } " << std::endl;

    yy::Tuple tuple{Struct{10}, std::string("vsb-tuo"), 6.9, false};

    std::cout << " { " << std::get<0>(tuple).x << ", " << std::get<1>(tuple) << ", " << std::get<2>(tuple) << ", " << std::boolalpha << std::get<3>(tuple) << " } " << std::endl;

    UniquePtr num = makeUnique(new int{35});
    UniquePtr str = makeUnique(new std::string{"vsb"});
    UniquePtr s = makeUnique(new S());

    std::cout << "num: " << *num << ": " << num.getNum() << std::endl;
    std::cout << "string: " << *str << ": " << str.getNum() << std::endl;

    std::vector<size_t> vec{1, 2, 3, 4, 5};
    auto sum = fold(vec.begin(), vec.end(), static_cast<size_t>(0),
                    [](size_t &acc, size_t el)
                    {
                        acc += el;
                    });

    for (decltype(auto) it : vec) // for (auto &it : vec)
    {
        it += 1;
    }

    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        *it += 1;
    }

    for (auto it : vec)
    {
        std::cout << it << ",";
    }
    std::endl(std::cout);

    std::cout << sum << std::endl;
}
