# Assignment
Implement a simplified version of a `numpy` array.

Implement a class called `NumpyArray`, which will hold a number of items of a generic type and allow
performing common operations on arrays with a simple syntax:

```cpp
NumpyArray<int> a{{1, 2, 3}};
NumpyArray<int> b{{3, 2, 4}};
auto c = a + b; // {4, 4, 7}
```

The array should support indexing, (lazy) iteration, addition and multiplication with other arrays
and scalar values, printing and simple N-dimensional indexing.
Same as in `numpy`, it should be possible to create a "view" into some part of the array by
creating a read-only slice (`NumpySlice`). This slice should support all read-only operations
offered by the array (const indexing, iteration, addition, printing).

Use templates to implement common operations for both arrays and slices without causing too much
code duplication.

Expected features:
- `NumpyArray` should be constructible from an `std::vector` of the contained generic type.
- `NumpyArray` and `NumpySlice` should support the iterator protocol. Iteration should not copy
the contents of the array/slice and should be lazy.
- `NumpyArray` and `NumpySlice` should support read-only item indexing using the bracket operator `[]`.
- `NumpyArray` should support mutable item indexing using the bracket operator `[]`.
- `NumpyArray` and `NumpySlice` should be printable to an output stream using the `<<` operator.
  Simply output the contained values separated by a comma and a space. See tests for details.
- `NumpyArray` and `NumpySlice` should offer the possibility to create read-only "views" (slices)
into their data by offering a `slice` method.

- The slice will behave like `std::string_view`, therefore if you change the values in the original
array, the changes will be reflected in the slice. The slice is read-only, but it's not a
copy of the original data!

  ```cpp
  NumpyArray<int> arr{{1, 2, 3, 4, 5}};
  auto s1 = arr.slice(1, 3); // slice "looks at" numbers [2, 3]
  arr[1] = 8; // change in the original array must be reflected in the slice
  assert(s1[0] == 8);
  ```
  
  Similar to Python and `numpy`, provide three variants of the method:
  - `slice()` - creates a slice over the whole array/slice
  - `slice(index)` - creates a slice starting at the given index and ending at the end of the array/slice
  - `slice(from, to)` - creates a slice starting at `from` and ending at `to`.
  `from` is inclusive, `to` is exclusive. If the user selects invalid indices, throw `ArrayException`
  (see tests for details).

- Allow adding and multiplying arrays with other arrays and slices. All combinations should be
possible (array + array, array + slice, slice + array, slice + slice). Addition and
multiplication of arrays/slices should always yield a `NumpyArray` instance.

Addition and multiplication is done in an element-wise manner:

  ```cpp
  NumpyArray<int> a{{1, 2, 3}};
  NumpyArray<int> b{{4, 0, 2}};
  a + b; // {5, 2, 5}
  a * b; // {4, 0, 6}
  ```

- Use a concept to only enable addition for types that can be added together, and multiplication for
types that can be multiplied together.

- Implement broadcasting (concept from `numpy`) to handle arrays of different sizes. For `a + b` where
`a.size() != b.size()`:
  1) If `a` has exactly one element, `a` will behave as if it contained `b.size()` values of `a[0]`.
  2) If `b` has exactly one element, `b` will behave as if it contained `a.size()` values of `b[0]`.
  3) Otherwise, throw an `ArrayException`.

- Allow addition of and multiplication by scalars for arrays and slices. Adding a scalar
  to an array/slice should behave in the same way as if the scalar was contained
  in a slice of size 1.

  ```cpp
  NumpyArray<int> a{{1, 2, 3}};
  a + 2; // {3, 4, 5}
  5 + a; // {6, 7, 8}
  ```

- Implement lazy transformations of iterables.

- Implement an `Iterable` class which will internally store two iterators (begin and end).
The `Iterable` class will have a constructor which takes two iterators (begin and end).
The iterators should be type parameters of `Iterable`.

- `NumpyArray<T>` and `Slice<T>` should implement the `iter()` method, which will return an
instance of `Iterable`.

- The `Iterable` class will have methods `filter(predicate)` and `map(predicate)`. Both methods
will return an iterable object (i.e. and object with methods `begin()` and `end()`). However, you
may want to create new classes for both filtering and mapping.

- The filtering and mapping classes will also implement `filter(predicate)` and `map(predicate)`.

`Iterable`, as well as the filtering and mapping class, will also have a `reduce` method, which
takes a reducer -- a function accepting a reference to an accumulator and an element of
the iterable, reduces the entire sequence, and returns the result. The accumulator should be
a reference so that it can be transformed in-place, without the need to copy the accumulator
or invoke `operator=` needlessly multiple times.

The initial value for the `reduce` accumulator will be the first element of the sequence.

You already know the `reduce` function from Haskell, so this should all be clear to you.

In the end, you should be able to chain transformations easily

```c++
NumpyArray<int> array { { 1, 2, 3, 4, 5, 6 } };

int i = array
    .iter()
    .filter([](int num) { return num % 2 == 0; })
    .map([](int num) { return num * 2; })
    .reduce([](int& acc, int num) { acc += num });
```

It is up to you how you implement the array, but try to keep code reuse high (try to avoid copy-pasting
implementations if it's not necessary). Instead, use proper generic programming using templates
to enable all necessary operations and methods on both arrays and slices.

Ideas for implementations:
1) Create a separate type for the slice.
2) Keep a single type (NumpyArray), but parametrize it with another type which will generalize the data storage:
- Array -> owned storage
- View -> borrowed storage
  Enable mutation only for the owned storage with template specialization.
3) Use inheritance with CRTP for a different implementation of 2).

Hint: Feel free to implement the iterators over slices and arrays using pointers or `std::vector`
iterators depending on your representation of the underlying data. You do not have to implement
your own iterators for the array/slice, as you'll enjoy a healthy dose of iterator implementation
in the lazy transformation portion of the assignment.

Bonus:
- For arrays and slices containing booleans, allow ANDing them together:

  ```cpp
  NumpyArray<bool> a{{true, false, true}};
  NumpyArray<bool> b{{false, true, true}};
  a && b; // {false, false, true}
  ```
  Only allow this for boolean arrays/slices! Again, support all array/slice combinations, broadcasting
  and scalars.
- Implement basic N-dimensional indexing functionality. In `numpy`, a 1D array can be treated as a 2D,
  3D or N-D array. The sizes of the individual dimensions are described with a "shape", and once you
  specify the shape, you can use multiple dimensional indexing to access parts of the array.
  - Add a second constructor argument (`std::vector<size_t>`) that will describe the individual dimensions
  of the stored data (the shape).
  - Add a `nd_index1` method, which will receive a list of indices (stored in `std::array`), it will
  calculate the proper 1D index that should be accessed, and returns a constant reference to the item
  at the corresponding index.
  - If the amount of indices passed to `nd_index1` does not match the shape, or any of the indices
  are too large for its given dimension, throw an `ArrayException`.
