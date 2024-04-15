#pragma once

#include <utility>

namespace yy
{

    namespace
    {
        template <int index, typename... Ts>
        struct tupleImpl
        {
            tupleImpl() = default;
        };

        template <int index>
        struct tupleImpl<index>
        {
            tupleImpl() = default;
        };

        template <int index, typename T, typename... Ts>
        struct tupleImpl<index, T, Ts...> : tupleImpl<index + 1, Ts...>
        {
            T item;

            tupleImpl() = default;
            tupleImpl(T &&item, Ts &&...args) : tupleImpl<index + 1, Ts...>(std::forward<Ts>(args)...),
                                                item(item) {}
        };

        template <int index, typename T, typename... Ts>
        T &getImpl(tupleImpl<index, T, Ts...> &t)
        {
            return t.item;
        }
    }

    template <typename... Ts>
    class Tuple : tupleImpl<0, Ts...>
    {
    public:
        Tuple() = default;
        Tuple(Ts &&...args) : tupleImpl<0, Ts...>(std::forward<Ts>(args)...) {}

        template <int index>
        auto &get()
        {
            tupleImpl<0, Ts...> &t = *this;
            return getImpl<index>(t);
        }
    };

}

namespace std
{
    template <int index, typename... Ts>
    auto &get(yy::Tuple<Ts...> &tuple)
    {
        return tuple.template get<index>();
    }
}