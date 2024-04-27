#pragma once

template <typename ...Ts> struct Stack {};

template <typename ...Ts, typename ...As>
constexpr auto push(Stack<Ts...>, As...) -> Stack<As..., Ts...>;

template <typename T, typename ...Ts>
constexpr auto pop(Stack<T, Ts...>) -> Stack<Ts...>;

template <typename T, typename ...Ts>
constexpr auto top(Stack<T, Ts...>) -> T;
