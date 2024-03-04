# Assignment
Your task will be to implement a set of parsers and parser combinators.
Parser combinators is an approach for building a complex parser (e.g. a JSON parser, but it can
parse pretty much anything) out of a set of small building blocks ("LEGO bricks") that can parse
simple things, like single characters. Using parser combinators, you then compose these simple
parsers together to construct more and more complex parsers easily.

Note: in this task, you will only implement a few of the most basic parsers and combinators
that won't be very useful. In a later task, we will modernize the code and add templates to it
to create very powerful parsers.

Basic terminology for this task:
- `Parser` is a function that receives a C string, parses some prefix of it and then:
  - If parsing fails, it returns `nullptr`.
  - If parsing succeeds, returns the rest of the string after the parsed part (where some next
  parser can continue).
  A Parser matches some input if a **prefix** of the input is recognized by it.
  It should not skip anything - if it does not find what it wants at the beginning of the input
  string, it should return a failed result.
- `Parser combinator` is a function that creates (returns) `Parser`s. It can also take other
  Parser(s) as parameters.

Create the following parsers and parser combinators:

- `create_char_parser` is a Parser combinator that receives a character `c` and creates a parser
    that recognizes that exact character.
    Example:
    ```cpp
    auto parser = create_char_parser('x');
    parser("y");   // null
    parser("x1");  // "1"
    ```

- `regex_match` is a parser used to match strings against a regular expression.

  Example:
  ```cpp
  auto parser = regex_match(R"[a-zA-Z]{3}\d{2,4}$");
  parser("abc1");   // null
  parser("abc12");  // ""
  parser("abc12d"); // null
  parser("1abc12"); // null
  ```

  Make sure to only accept the string if the match occurs at the very beginning of the string.
  Use `std::regex`, `std::cmatch` and `std::regex_search` to implement this parser. Avoid
  using `regex_match` to implement other parsers when possible for performance reasons.

- `skip_ws` is a parser which skips all consequent whitespace characters, if the input string
  starts with any such characters. If the string does not start with whitespace characters,
  return an unmodified string.

  ```cpp
  auto parser = skip_ws();
  parser("  123");       // "123"
  parser("\r\n\t  123"); // "123"
  parser("123 456");     // "123 456"
  ```

- `any_of` is a Parser combinator that receives a vector of Parsers and returns a Parser that
  will try to match the input parsers one by one and return the first found match
  (or null if no parser matches).
    Example:
    ```cpp
    auto parser = any_of({create_char_parser('x'), create_char_parser('y')});
    parser("a");  // null
    parser("xb"); // "b"
    parser("yz"); // "z"
    ```

- `sequence` is a parser combinator that receives a vector of Parsers and returns a Parser that
  will try to match all the input parsers, applied one after another. It will match if all the
  input parsers match in succession.
    Example:
    ```cpp
    auto parser = sequence({
        create_char_parser('x'),
        any_of({create_char_parser('y'),
        regex_match(R"\d")}),
        create_char_parser('z')
    });
    parser("a");    // null
    parser("x1x");  // null
    parser("xyzu"); // "u"
    parser("x2zy"); // "y"
    ```

- `repeat` is a Parser combinator that receives a Parser and a number of repetitions `n` and
  returns a parser that will repeat the input parser `n` times.
  Try to leverage one of the aforementioned parsers to implement this combinator, but avoid
  using `regex_match`.
    Example:
    ```cpp
    auto parser = repeat(create_char_parser('a'), 3);
    parser("a");    // null
    parser("1aaa"); // null
    parser("aa");   // null
    parser("aaaa"); // "a"
    ```

- `create_word_parser` is a Parser combinator that receives a string and returns a Parser that
  will recognize that string.
  Try to leverage one of the aforementioned parsers to implement this combinator, but avoid
  using `regex_match`.
    Example:
    ```cpp
    auto parser = create_word_parser("foo");
    parser("a");    // null
    parser("f");    // null
    parser("fo");   // null
    parser("foox"); // "x"
    ```
