# Assignment
Your task will be to implement a class that will represent a growable string containing UTF-8-encoded Unicode code points.
The string will always represent the text data with UTF-8 bytes to save memory, but it will be able to reconstruct the Unicode code points on demand.

`UTF8String` should be a RAII container that will manage (UTF-8) byte data stored on the heap.
Do not use std::string/vector or any smart pointers to implement the byte storage. Instead,
allocate, reallocate and deallocate memory manually (we will use smart pointers later).

Implement constructor, destructor, copy constructor and copy assignment constructor (rule of three) to avoid leaking data and properly manage lifetime of the allocated bytes (we will add move constructors later).

The string is growable, so it has to support dynamic adding of new chars or code points. Use a reasonable reallocation strategy to avoid reallocating on each new char/code point addition.

The string must support the following operations:
- Can be created from a C or C++ string that contains ASCII bytes.
```cpp
UTF8String str{"ahoj"}; // holds bytes 0x61, 0x68, 0x6F, 0x6A
```
- Can be created from a vector of code points.
```cpp
UTF8String str{std::vector<CodePoint>{ 0x61, 0x20AC }}; // holds bytes 0x61, 0xE2, 0x82, 0xAC
```
- Can be copied and copy assigned.
```cpp
UTF8String str{"ahoj"};
UTF8String str2{str}; // copy
str2 = str;           // copy assignment
```
- Returns the number of currently stored bytes and codepoints.
The codepoints should not be stored directly in the string! They need to be recalculated on each method call.
```cpp
UTF8String str{std::vector<CodePoint>{ 0x61, 0x20AC }};
str.get_byte_count();  // 4
str.get_point_count(); // 2
```
- Can be indexed by bytes using the `[]` operator.
If the index is invalid, return `std::nullopt`.
The time complexity of this method should be `O(n)`.
```cpp
UTF8String str{std::vector<CodePoint>{ 0x61, 0x20AC }};
str[0]; // 0x61
str[1]; // 0xE2
str[2]; // 0x82
str[3]; // 0xAC
str[4]; // std::nullopt
```
- Can be indexed by code points using the `nth_code_point` method.
If the index is invalid, return `std::nullopt`.
The time complexity of this method should be `O(n)`.
```cpp
UTF8String str{std::vector<CodePoint>{ 0x61, 0x20AC }};
str.nth_code_point(0); // 0x61
str.nth_code_point(1); // 0x20AC
str.nth_code_point(2); // std::nullopt
```
- Can add new (ASCII) chars using the `append` method.
```cpp
UTF8String str{"ahoj"};
str.append(' ');
str.append('x');
```
- Can add new code points using the `append` method.
```cpp
UTF8String str{"ahoj"};
str.append(static_cast<CodePoint>(0x1F601));
```
- Two strings can be concatenated together using the `+` operator.
  This will create a new string, the original strings should stay as they were!
```cpp
UTF8String str{"Hello"};
UTF8String str2{" world"};
str + str2; // UTF8String containing "Hello world"
```
- A string can be appended to an already existing string using the `+=` operator. This will modify the left hand string rather than create a new object.
```cpp
UTF8String str{"Hello"};
UTF8String str2{" world"};
str += str2;
// str == UTF8String{"Hello world"}
```
- Can be compared to other UTF8Strings. Two strings are considered equal if their UTF-8 byte array
is equal.
```cpp
UTF8String str{"ahoj"};
UTF8String str2{"cau"};
UTF8String str3{std::vector<CodePoint>{ 0x61, 0x68, 0x6F, 0x6A }};
*
str == str2; // false
str == str3; // true
```
- Can be converted to an `std::string` using an explicit cast. In this case, assume that the string contains only ASCII characters.
```cpp
UTF8String str{"ahoj"};
std::string str2 = static_cast<std::string>(str);
// str2 == "ahoj"
```
