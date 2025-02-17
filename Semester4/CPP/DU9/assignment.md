# Assignment
Implement several compile-time metaprogramming tasks.
Use https://cppinsights.io/ to gain insight into what the compiler is doing.
There is no need to execute your programs :) Just compile them.

## Compile-time matrix
Implement a compile-time Matrix that can be added and multiplied together.
The matrix should be generic (use a template parameter for the contained type).
The row and column counts of the matrix are compile-time constants.
What type from STL can be used to store the elements of the matrix?

Implement:

1) `at` method, which will receive a row and a column and return the item at that position.
It needs to work at compile-time.
2) Matrix addition of two matrices using the `+` operator. If the matrices have a different type or
their row or column count isn't equal, the code should not compile.
    Example:

    ```cpp
    constexpr Matrix<int, 2, 3> a{{1, 2, 3, 4, 5, 6}};
    constexpr Matrix<int, 2, 3> b{{1, 1, 1, 1, 1, 1}};
    constexpr auto result = a + b; // type of `result` is Matrix<int, 2, 3>, values are {2, 3, 4, 5, 6, 7}
    // Matrix<int, 3, 3> c = a + b; // doesn't compile
    ```
3) Matrix multiplication of two matrices using the `*` operator. The matrices need to share the
"inner" dimension (https://www.mathsisfun.com/algebra/matrix-multiplying.html).
    Example:

    ```cpp
    constexpr Matrix<int, 2, 3> a{{1, 2, 3, 4, 5, 6}};
    constexpr Matrix<int, 3, 1> b{{0, 2, 3}};
    constexpr auto result c = a * b; // type of `result` is Matrix<int, 2, 1>, values are {13, 28}
    ```

## Linked-list
Implement a compile-time linked list.
Each list node is either `Null` (end/empty) or `List` containing a single number.
The list should be constructible like this:
```cpp
using MyList = List<1, List<2, List<3, List<4, Null>>>>;
```

Implement the following operations on linked lists. Each operation will be a struct that takes
a list as a template parameter and calculates some value. The value will be stored in a static
variable called `Value`.

1) `ListFirst`: return the first value (number) of the list. If the list is empty, this should not
compile.
    ```cpp
    ListFirst<List<1, List<2, Null>>>::Value == 1
    ListFirst<Null>::Value // doesn't compile
    ```
2) `ListLast`: return the last value (number) of the list. If the list is empty, this should not
compile.
    ```cpp
    ListLast<List<1, List<2, Null>>>::Value == 2
    ListLast<Null>::Value // doesn't compile
    ```
3) `ListLength`: return the length (count of non-null nodes) of the list.
    ```cpp
    ListLength<Null>::Value == 0
    ListLength<List<1, Null>>::Value == 1
    ListLength<List<1, List<2, Null>>>::Value == 2
    ```
4) `ListSum`: return the sum of all values (numbers) of the list.
    ```cpp
    ListSum<Null>::Value == 0
    ListSum<List<1, Null>>::Value == 1
    ListSum<List<1, List<2, Null>>>::Value == 3
    ```
5) `ListMax`: return the maximum value (number) of the list. If the list is empty, this should not
compile.
    ```cpp
    ListMax<Null>::Value // doesn't compile
    ListMax<List<1, Null>>::Value == 1
    ListMax<List<1, List<5, List<3, Null>>>>::Value == 5
    ```
6) `ListReverse`: reverse the linked list. The result will be a type representing the list with
reversed order of types. Remember: how to pass temporary state between function calls in Haskell?
We need to convert local variables (the reversed list) into an (output) parameter.

`ListReverse<...>::Value` will be a type that will represent the reversed list. You can create
such output type using `using` :)
You will probably need to add `typename` in various places to help the compiler recognize
that some inner member of a struct is indeed a type.

```cpp
ListReverse<Null>::Value                    // Null
ListReverse<List<1, Null>>::Value           // List<1, Null>
ListReverse<List<1, List<2, Null>>>::Value  // List<2, List<1, Null>>
```

## Program interpreter
Implement a simple compile-time program interpreter.
The interpreter will work with a stack of instructions (instruction stack)
and a stack of values (value stack).
Both instructions and values will be represented with types.

Implement the following values:

1) `Number`: contains a `size_t` value.
    ```cpp
    Number<1>
    Number<5>
    ```
2) `Bool`: contains a `bool` value.
    ```cpp
    Bool<true>
    Bool<false>
    ```

Implement the `Interpret` struct, which will receive the instruction stack and value stack as
template parameters, it will execute the given instructions from the instruction stack and return
the result of the interpreted program in a static variable named `Result`.

The interpreter will read instructions from the instruction stack one by one, execute them and
modify the value stack. The program ends successfully if it gets to a state where the instruction
stack is empty and there is exactly one value on the value stack.
- If the last value on the value stack is `Number`, the result of the program will be `size_t` with
the value of the `Number`.
- If the last value on the value stack is `Bool`, the result of the program will be `bool` with
the value of the `Bool`.

The interpreter will work with various instructions that are described below. Implement the
following instructions:

1) `PushNumber`: contains a compile-time `size_t` value `N`. Push a `Number` containing `N` to the
value stack.
    ```cpp
    // Stack<PushNumber<5>>, Stack<>
    // v
    // Stack<>, Stack<Number<5>>
    Interpret<Stack<PushNumber<5>>, Stack<>>::Value == 5
    ```
2) `PushBool`: contains a compile-time `bool` value `B`. Push a `Bool` containing `B` to the
value stack.
    ```cpp
    // Stack<PushBool<true>>, Stack<>
    // v
    // Stack<>, Stack<Bool<true>>
    Interpret<Stack<PushBool<true>>, Stack<>>::Value == true
    ```
3) `Add`: pop two numbers off the value stack, add them together and push the
result back onto the value stack. If there are not two numbers at the top of the value stack,
the program should not compile.
    ```cpp
    // Stack<PushNumber<5>, PushNumber<3>, Add>, Stack<>
    // v
    // Stack<PushNumber<3>, Add>, Stack<Number<5>>
    // v
    // Stack<Add>, Stack<Number<3>, Number<5>>
    // v
    // Stack<>, Stack<Number<8>>
    Interpret<Stack<PushNumber<5>, PushNumber<3>, Add>, Stack<>>::Value == 8
    ```
4) `Max`: pop two numbers off the value stack, and push (only) the larger of them
back onto the value stack. If there are not two numbers at the top of the value stack,
the program should not compile.
    ```cpp
    // Stack<PushNumber<5>, PushNumber<3>, Max>, Stack<>
    // v
    // Stack<PushNumber<3>, Max>, Stack<Number<5>>
    // v
    // Stack<Max>, Stack<Number<3>, Number<5>>
    // v
    // Stack<>, Stack<Number<5>>
    Interpret<Stack<PushNumber<5>, PushNumber<3>, Max>, Stack<>>::Value == 5
    ```
5) `Pop`: pop a value (either number or a bool) off the value stack. If there is no value at the
top of the value stack, the program should not compile.
    ```cpp
    // Stack<PushNumber<5>, Pop>, Stack<>
    // v
    // Stack<Pop>, Stack<Number<5>>
    // v
    // Stack<>, Stack<>
    *
    // Stack<PushBool<true>, Pop>, Stack<>
    // v
    // Stack<Pop>, Stack<Bool<true>>
    // v
    // Stack<>, Stack<>
    ```
6) `Compare`: pop two numbers off the value stack and compare them. If they are equal, push
`Bool<true>` to the value stack. Otherwise push `Bool<false>` to the value stack. If there are
not two numbers at the top of the value stack, the program should not compile.
    ```cpp
    // Stack<PushNumber<5> PushNumber<5>, Compare>, Stack<>
    // v
    // Stack<PushNumber<5>, Compare>, Stack<Number<5>>
    // v
    // Stack<Compare>, Stack<Number<5>, Number<5>>
    // v
    // Stack<>, Stack<Bool<true>>

    // Stack<PushNumber<5> PushNumber<4>, Compare>, Stack<>
    // v
    // Stack<PushNumber<4>, Compare>, Stack<Number<5>>
    // v
    // Stack<Compare>, Stack<Number<4>, Number<5>>
    // v
    // Stack<>, Stack<Bool<false>>
    ```
7) `Select`: pop a boolean value off the value stack and make sure that there are at least two
instructions `A` and `B` following the `Select` in the instruction stack.
If the boolean value is true, execute `A`, skip executing `B` and then continue with the program.
If the boolean value is false, execute `B`, skip executing `A` and then continue with the program.
If there is not a bool at the top of the value stack, or if the instruction stack has less than
two instructions after `Select`, the program should not compile.
    ```cpp
    // Stack<PushBool<true>, Select, PushNumber<5>, PushNumber<3>, PushNumber<1>>, Stack<>
    // v
    // Stack<Select, PushNumber<5>, PushNumber<3>, PushNumber<1>>, Stack<Bool<true>>
    // v
    // Stack<PushNumber<1>>, Stack<Number<5>>
    
    // Stack<PushBool<false>, Select, PushNumber<5>, PushNumber<3>, PushNumber<1>>, Stack<>
    // v
    // Stack<Select, PushNumber<5>, PushNumber<3>, PushNumber<1>>, Stack<Bool<false>>
    // v
    // Stack<PushNumber<1>>, Stack<Number<3>>
    ```

In `tasks.h`, you can find an implementation of a compile-time stack using variable generics that
you can use. It's functionally equivalent to `List` above, but using variable generics makes it a
bit easier to use.

See tests for usage.
