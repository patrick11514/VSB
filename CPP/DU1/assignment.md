# Assignment
Implement a simple set of C++ functions.

Put their declaration into `tasks.h` and their implementation into `tasks.cpp`.

Implement the following functions:
- Implement a function named `fibonacci` inside namespace `math`.
    It will receive a single integer parameter `n` and it will return the `n-th` Fibonacci number (counted from zero).
    
    Example:
    ```cpp
     math::fibonacci(0); // 0
     math::fibonacci(1); // 1
     math::fibonacci(2); // 1
     math::fibonacci(3); // 2
    ```
- Implement a function named `camel_to_snake` inside namespace `strutils`.
  It will receive a string in camelCase and should return the input string converted to snake_case.
  Example:
  ```cpp
  strutils::camel_to_snake("helloWorld); // "hello_world"
  ```

- Implement a function named `parse_uint` inside namespace `strutils`.
  It will receive a string, it should parse that string as a 32-bit unsigned int.
  If the parsing is successful, the function should return `true`. Otherwise, it will return `false`.

  For the parsing to be successful, the input string has to contain a valid decimal representation of a 32-bit unsigned integer.
  If it contains any character other than a digit (0-9) anywhere in the string, parsing fails.
  If the string is empty, parsing fails.
  If the contained number is too large (it overflows 32-bit unsigned integer), parsing fails.

  Note: DO NOT use any library function (atoi, strtol, from_chars, etc.) when implementing this function. Try to implement it "from scratch" :)

  Example:
  ```cpp
  strutils::parse_uint("helloWorld", result);         // false
  strutils::parse_uint("123", result);                // true, result == 123
  strutils::parse_uint("999999999999999999", result); // false
  ```

- Implement a function named `validate_utf8` inside namespace `strutils`, which will receive a vector of bytes (`uint8_t`).
  It will validate if the byte sequence is a valid representation of some UTF-8 string and if yes, it will count the number of Unicode code points contained within the string and return them via an output parameter (l-value reference).
  If the input bytes do not form a valid UTF-8 string, the passed reference should not be modified!

  See https://en.wikipedia.org/wiki/UTF-8#encoding for details.

  Example:
  ```cpp
  size_t result;
  // true, result = 1
  strutils::validate_utf8({0b00001100}, result);
  // true, result = 2
  strutils::validate_utf8({0b11000011, 0b10001101, 0b00001001}, result);
  // false
  strutils::validate_utf8({0b10001001}, result);
  ```
