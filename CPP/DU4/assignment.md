# Assignment
Your task will be to implement a hierarchy describing a JSON document.
This is an exercise in modelling a document (a hierarchy containing various types of elements)
using a vanilla OOP approach with polymorphism.
You are welcome to try to also model this document with another approach, e.g. using `std::variant`.
We will see later that such approach might be more ergonomic.

An abstract document element will be represented by the `Value` class. Use OOP to model JSON
documents that can contain:
1) An integer (`Integer`)
2) An array of JSON elements (`Array`)
3) An object with string keys (`Object`)
4) A null value representing a lack of value (`Null`)

Below you can find comments regarding the implementation and required features (also see tests).
Try to think about the trade-offs that you encounter while implementing the document using OOP
and include a comment in the submission that will describe your experiences.

There are some questions marked with [*] -- copy these questions to some comment in `tasks.h` and
answer them in your submission (you can also answer them using the Kelvin comment system).

# Memory management
You can use STL containers for storing the array items (e.g. std::vector) and the object items
(some kind of map or a tree). Do not use smart pointers yet, store raw pointers inside the container
elements (array/object) and handle memory deallocation properly. The container elements should
receive its children as raw pointers and at that moment they will become unique owners of these
pointers (they will be responsible for their deletion).
You don't have to implement copy/assignment support for the document, instead make sure to disable
copying/assignment for it entirely.
[*] Can individual elements be shared among different JSON containers (array/object) with the mentioned
design? Can you take one specific element (e.g. an integer) allocated on the heap and place it inside
an array and also inside an object?
[*] Could references (or non-owned pointers) be used for storing the elements in JSON containers?
How would it affect the usability of arrays and objects? Try it and see how easy or hard it is :)

# Indexing
It will be possible to index each JSON element both with a numeric index and with a string
using the `[]` operator. These index operations will be read-only.
[*] Non-indexable objects will throw an exception when you attempt to index them using `operator[]`.
Feel free to use an exception from the standard library, but prefer creating a custom exception
if you can. Use a name you find suitable for the exception, the tests only check that an exception
is thrown, not what kind of exception it is.
[*] Think about this interface. What are its benefits or disadvantages? Is it better to put the
indexers into the root `Value` element? Or should it only be supported by types that actually
implement indexing (arrays/objects)? Think about the trade-offs (compile-time safety vs ergonomics).
[*] Think about the return type. What should it be? Is `std::optional` required here?
    
# Copying
It will be possible to copy (clone) each JSON element using a `clone` method. This will introduce
a deep copy, copying all contained elements. See the `Prototype` design pattern.
[*] How can you implement copying for a polymorphic object hierarchy? What should be the return type
of the `clone` method? If you are interested, look for "C++ covariance".

# Integers
Integer objects will have a `get_value` method that will return their actual value.

# Arrays
Arrays can be constructed from a sequence of Values.
Arrays will have a `size` method that will return the count of their child elements.
Array will have an `append` method that will add a new element to the end of the array.
Arrays will have a `remove` method which will remove an element at a specified index.

# Objects
Objects can be constructed from a sequence of pairs (key, value) or from a dictionary that maps
string keys to values.
Objects will have a `keys` method that will return their keys as `std::vector<std::string>`.
The returned keys will be sorted in an increasing alphabetical order (use `std::sort`).
Objects will have an `insert` method that will receive a key and an element, and it will add
that element to the object. Make sure to handle situations where the key was already present.
Objects will have a `remove` method which will remove an item under a specified key.

Hint: "dictionary" in C++ standard library can be found under the name `std::unordered_map`.

# Visitor
The final task is to allow adding new behaviour to the JSON document class hierarchy without changing
its source code. We will use the `Visitor` design pattern (https://en.wikipedia.org/wiki/Visitor_pattern)
for this.

Hint:
Create a class representing a visitor and add methods for visiting each type of JSON element to it.
Then implement a virtual method `accept` in each JSON element class that will delegate to the correct
visitor method and pass itself to it.

[*] Think about the constness of the `accept` method and of the methods in the visitor. What
parameter type should they take? Should it be const or not? Consider creating two classes
to represent visitors, `Visitor` and `MutatingVisitor` (or similar names).

Then implement these two simple visitors:
`RemoveNullVisitor` will receive a JSON element and remove all `null` values from objects and arrays.

`PrintVisitor` will receive a JSON element and a stream, and it will print the element on a single
line to that stream. The output format should correspond to common JSON representation.
With this visitor it should be simple to overload the `<<` operator for JSON elements and allow
them to be printed e.g. to `stdout`.

[*] How would the implementation look like if you have used e.g. algebraic data types instead?
Would you need the Visitor design pattern in that case?

See tests for usage examples of all described APIs.
