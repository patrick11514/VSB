#include <iostream>
#include <string>
#include <vector>

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

int main()
{
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
