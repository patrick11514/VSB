# Assignment
Implement the tasks below. The primary task is to implement the streaming JSON parser.

## Streaming JSON parsing
Implement a simplified streaming JSON parser.

To represent the JSON document, we have prepared an algebraic data type representation of it.
This should make it a bit simpler to work with (compard to the previous OOP implementation with virtual
methods), because you will not have to deal with heap allocation explicitly.
It has also been extended to match JSON better (although it's not fully precise yet - e.g. strings
aren't UTF-8).

1) Implement printing of JSON values to an output stream using the `<<` operator.
Print values with their natural JSON representation, no special indenting or pretty printing
is necessary (everything will be on the same line). See tests for details. You can use `std::visit`
to examine the contents of the passed JSON value.

    Implement the `std::ostream& operator<<(std::ostream& os, const Value& value)` function.

2) **Main task** Implement a simplified version of a streaming JSON parser in the `parse_json`
function. The function will take an input stream, and it will try to parse it as JSON. If the
input cannot be parsed as JSON, the function will return `std::nullopt`.

You can find state machines describing the JSON syntax at https://www.json.org/json-en.html.
You don't have to implement the full specification, just a simplified version described below:

- Null: `null`
- Boolean: `true` or `false`
- String: ASCII characters wrapped in quotes (`"`). The string can contain characters `\` and `"`
escaped with a backslash (`\`). You don't have to implement escaping of any other characters nor
add support for UTF-8. If a backslash escapes anything other than `\` or `"`, it is an error.

  Example:
  - `"a"` -> `a`
  - `"ab cd"` -> `ab cd`
  - `"a\\x"` -> `a\x`
  - `"a\"x"` -> `a"x`
  - `"\x"` -> `(invalid)`

- Number: Integers and decimal numbers.
  - Can be negative (`-5`).
  - Can contain a single decimal point (`123.5`), but they cannot start with a decimal point.
  - Scientific notation (10e-5) does not have to be supported.
  - Try to parse the number manually, without using e.g. `std::stof` or similar functions.

- Array: Comma-separated JSON values wrapped in square brackets (`[]`).
  Example:
  ```json
  []
  [1, 2, 3]
  [["foo", 1], [], [true], null, {"x": [1]}]
  ```
  - Trailing comma is not supported (`[1, 2,]` is an error).
  - Multiple commas without a value is also an error (`[1,,2]`).

- Object: Comma-separated key-value pairs wrapped in brackets (`{}`).
  - The key has to be a string, otherwise it's an error (`{1: true}` is invalid).
  - Trailing comma is not supported (`{"a": true,}`) is an error.
  - Multiple commas without a value is also an error (`{"a": 1,, "b": 2}`).

The rest should correspond to the JSON specification.

**General rules**:
- Each JSON value can contain spaces before it, there can also be spaces aroung braces (`{}`, `[]`),
commas (`,`) and colons (`:`). JSON considers multiple characters as whitespace (newline, tabs, space,
...), here it's enough to support spaces and newlines.
- The parser should work in the same way as the parsers from the `Functional programming` lesson,
therefore it should parse some prefix of the input stream. For example, `123true` should be parsed
as the number `123` and it should leave the content `true` in the input stream. If you want to, you
can reuse and extend the parsers from the previous lesson to parse JSON, although it might require a
lot of work to build enough parser combinators to support parsing JSON.  
- **The parser has to be streaming!!!** It cannot afford to "go back" in the stream (what if the stream
is coming from a network socket?) and it cannot afford to load the whole stream into memory (what
if it's larger than your RAM?). For example, you are not allowed to read the whole stream into
a string and then parse the string.

  Think about how you can parse the items in a streaming fashion. You can't start parsing one
  type of element and then stop in the middle if you find an error any try another type, because
  you would have consumed part of the stream in the meantime (well, you could do it by buffering
  the parsed part, but it's quite complicatedm, and you could run out of memory :) ). Try to find what
  type of element could be in the input without consuming any characters.

  Hint: the `std::istream::peek()` function should be quite useful.
- Implement the parser yourself (using natural, not artificial intelligence), it's not an acceptable solution if you use a JSON library for
it :)

## Binary serialization/deserialization of JSON values

Implement binary serialization and deserialization of JSON values. Use an `std::vector<uint8_t>` to
represent binary data.

To encode a JSON value, first use a single byte to determine the type of the following value.
Use the following type IDs to specify each type.

| Type   | ID |
|--------|----|
| Null   | 0  |
| Bool   | 1  |
| Number | 2  |
| String | 3  |
| Array  | 4  |
| Object | 5  |

### Encoding Null

Null will only be represented using it's type ID, since, obviously, you do not need any additional data
to encode a null value.

### Encoding Bools

The type ID will be followed by a single byte. The value `true` will be encoded using the value `1`,
the value `false` will be encoded using the value `0`.

This is not the most efficient way to represent boolean values. Since there's only a limited number
of objects, we could, for example, only use four bits to represent the type ID, and use the remaining
four to encode booleans. However, we will stick to the aforementioned encoding for the purpose
of simplicity.

### Encoding Numbers

The type ID will be followed by 8 bytes encoding a double precision floating point number.

### Encoding Strings

The type ID will first be followed by an 8 byte unsigned integer (`size_t`) representing the size
of the string, and then by a sequence of ASCII characters. The serialized string should not be null
terminated, as we already know the length of the string.

### Encoding Arrays

To encode an array, you will first write the type ID, followed by the size of the array (`size_t`).
The size will be succeeded by binary encoded elements of said array.

### Encoding Objects

Much like arrays, binary representation of objects will consist of the type ID followed by the size
of the object. Each attribute of the object will be composed of a string representing the key,
immediately followed by the value of said attribute.

Do not prepend a type ID to the string. Since the key will always be a string, you can only serialize
the size and the key itself.

Since an `std::map` is used to represent an object, keys should be inherently sorted alphabetically.
When serializing an object, you can iterate over the map using iterators and serialize the key-value
pairs one by one. The result, if everything else is implemented correctly, should then match the
serialized objects found in tests.

### Error handling

Feel free to take the liberty of writing error handling free binary serialization/deserialization
code. Assume the serialized data will always contain valid JSON values. Of course, this only
applies to binary (de)serialization.

*Hint*: when implementing deserialization, you can pass around an iterator pointing to the start
of the object you're deserializing.
