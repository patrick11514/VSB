import dataclasses
from typing import Callable, Generic, List, Optional, TypeVar


def cached(f: Callable):
    """
    Create a decorator that caches up to 3 function results, based on the same parameter values.

    When `f` is called with the same parameter values that are already in the cache, return the
    stored result associated with these parameter values. You can assume that `f` receives only
    positional arguments (you can ignore keyword arguments).

    When `f` is called with new parameter values, forget the oldest inserted result in the cache
    if the cache is already full.

    Example:
        @cached
        def fn(a, b):
            return a + b # imagine an expensive computation

        fn(1, 2) == 3 # computed
        fn(1, 2) == 3 # returned from cache, `a + b` is not executed
        fn(3, 4) == 7 # computed
        fn(3, 5) == 8 # computed
        fn(3, 6) == 9 # computed, (1, 2) was now forgotten
        fn(1, 2) == 3 # computed again, (3, 4) was now forgotten
    """
    cache = {}
    keyList = []

    def newFunction(*args):

        strs = []

        for a in args:
            strs.append(str(a))

        key = ",".join(strs)
        print(f"KEY: {key}")
        print(keyList)
        print(cache)

        if key in cache:
            print("Returing cached result")
            return cache[key]

        if len(cache.keys()) == 3:
            first = keyList.pop(0)
            print("DELETING " + first)
            del cache[first]

        result = f(*args)
        cache[key] = result
        keyList.append(key)
        return result

    return newFunction


T = TypeVar("T")


@dataclasses.dataclass
class ParseResult(Generic[T]):
    """
    Represents result of a parser invocation.
    If `value` is `None`, then the parsing was not successful.
    `rest` contains the rest of the input string if parsing was succesful.
    """

    value: Optional[T]
    rest: str

    @staticmethod
    def invalid(rest: str) -> "ParseResult":
        return ParseResult(value=None, rest=rest)

    def is_valid(self) -> bool:
        return self.value is not None


"""
Represents a parser: a function that takes a string as an input and returns a `ParseResult`.
"""
Parser = Callable[[str], ParseResult[T]]

"""
Below are functions that create new parsers.
They should serve as LEGO blocks that can be combined together to build more complicated parsers.
See tests for examples of usage.

Note that parsers are always applied to the beginning of the string:
```python
parser = parser_char("a")
parser("a")  # ParseResult(value="a", rest="")
parser("xa") # ParseResult(value=None, rest="xa")
```
"""


def parser_char(char: str) -> Parser[str]:
    """
    Return a parser that will parse a single character, `char`, from the beginning of the input
    string.

    Example:
        ```python
        parser_char("x")("x") => ParseResult(value="x", rest="")
        parser_char("x")("xa") => ParseResult(value="x", rest="a")
        parser_char("y")("xa") => ParseResult(value=None, rest="xa")
        ```
    """

    if len(char) == 0 or len(char) > 1:
        raise ValueError

    def Parser(string: str):
        if len(string) == 0:
            return ParseResult(None, "")

        if string[0] == char:
            return ParseResult(string[0], string[1::])

        return ParseResult(None, string)

    return Parser


def parser_repeat(parser: Parser[T]) -> Parser[List[T]]:
    """
    Return a parser that will invoke `parser` repeatedly, while it still matches something in the
    input.

    Example:
        ```python
        parser_a = parser_char("a")
        parser = parser_repeat(parser_a)
        parser("aaax") => ParseResult(value=["a", "a", "a"], rest="x")
        parser("xa") => ParseResult(value=[], rest="xa")
        ```
    """

    def Parser(string: str):
        values = []
        valid = True

        while valid:
            result = parser(string)

            if result.is_valid():
                values.append(result.value)
            else:
                valid = False

            string = result.rest

        return ParseResult(values, string)

    return Parser


def parser_seq(parsers: List[Parser]) -> Parser:
    """
    Create a parser that will apply the given `parsers` successively, one after the other.
    The result will be successful only if all parsers succeed.

    Example:
        ```python
        parser_a = parser_char("a")
        parser_b = parser_char("b")
        parser = parser_seq([parser_a, parser_b, parser_a])
        parser("abax") => ParseResult(value=["a", "b", "a"], rest="x")
        parser("ab") => ParseResult(value=None, rest="ab")
        ```
    """

    def Parser(string: str):
        values = []
        valid = True
        input = string

        for parser in parsers:
            result = parser(string)

            if result.is_valid():
                values.append(result.value)
            else:
                valid = False
                break

            string = result.rest

        if not valid:
            return ParseResult(None, input)

        return ParseResult(values, string)

    return Parser


def parser_choice(parsers: List[Parser]) -> Parser:
    """
    Return a parser that will return the result of the first parser in `parsers` that matches something
    in the input.

    Example:
        ```python
        parser_a = parser_char("a")
        parser_b = parser_char("b")
        parser = parser_choice([parser_a, parser_b])
        parser("ax") => ParseResult(value="a", rest="x")
        parser("bx") => ParseResult(value="b", rest="x")
        parser("cx") => ParseResult(value=None, rest="cx")
        ```
    """

    def Parser(string: str):
        input = string
        rootResult = None

        for parser in parsers:
            result = parser(string)

            string = result.rest

            if result.is_valid():
                rootResult = result.value
                break

        if rootResult == None:
            return ParseResult(None, input)

        return ParseResult(rootResult, string)

    return Parser


R = TypeVar("R")


def parser_map(parser: Parser[R], map_fn: Callable[[R], Optional[T]]) -> Parser[T]:
    """
    Return a parser that will use `parser` to parse the input data, and if it is successful, it will
    apply `map_fn` to the parsed value.
    If `map_fn` returns `None`, then the parsing result will be invalid.

    Example:
        ```python
        parser_a = parser_char("a")
        parser = parser_map(parser_a, lambda x: x.upper())
        parser("ax") => ParseResult(value="A", rest="x")
        parser("bx") => ParseResult(value=None, rest="bx")

        parser = parser_map(parser_a, lambda x: None)
        parser("ax") => ParseResult(value=None, rest="ax")
        ```
    """

    def Parser(string: str):
        result = parser(string)

        if result.is_valid():
            mapResult = map_fn(result.value)

            if mapResult == None:
                return ParseResult(None, string)

            return ParseResult(mapResult, result.rest)

        return result

    return Parser


def parser_matches(filter_fn: Callable[[str], bool]) -> Parser[str]:
    """
    Create a parser that will parse the first character from the input, if it is accepted by the
    given `filter_fn`.

    Example:
        ```python
        parser = parser_matches(lambda x: x in ("ab"))
        parser("ax") => ParseResult(value="a", rest="x")
        parser("bx") => ParseResult(value="b", rest="x")
        parser("cx") => ParseResult(value=None, rest="cx")
        parser("") => ParseResult(value=None, rest="")
        ```
    """

    def Parser(string: str):
        if len(string) == 0:
            return ParseResult(None, "")

        first = string[0]
        if not filter_fn(first):
            return ParseResult(None, string)

        return ParseResult(first, string[1::])

    return Parser


# Use the functions above to implement the functions below.


def parser_string(string: str) -> Parser[str]:
    """
    Create a parser that will parse the given `string`.

    Example:
        ```python
        parser = parser_string("foo")
        parser("foox") => ParseResult(value="foo", rest="x")
        parser("fo") => ParseResult(value=None, rest="fo")
        ```
    """

    def Parser(innerString: str):
        inputString = innerString
        outputString = ""
        valid = True

        for i in range(len(string)):
            result = parser_matches(lambda x: x == string[i])(innerString)
            if result.is_valid():
                outputString += innerString[0]
                innerString = innerString[1::]
            else:
                valid = False
                break

        if not valid:
            return ParseResult(None, inputString)
        return ParseResult(outputString, innerString)

    return Parser


def parser_int() -> Parser[int]:
    """
    Create a parser that will parse a non-negative integer (you don't have to deal with
    `-` at the beginning).

    Example:
        ```python
        parser = parser_int()
        parser("123x") => ParseResult(value=123, rest="x")
        parser("foo") => ParseResult(value=None, rest="foo")
        ```
    """

    def Parser(innerString: str):
        inputString = innerString
        outputNumber = 0
        valid = True

        for _ in range(len(inputString)):
            parser = parser_matches(lambda x: ord(x) >= ord("0") and ord(x) <= ord("9"))
            result = parser(innerString)

            if result.is_valid():
                outputNumber = outputNumber * 10 + int(result.value)
                innerString = innerString[1::]
            else:
                valid = False
                break

        if not valid and outputNumber == 0:
            return ParseResult(None, inputString)

        return ParseResult(outputNumber, innerString)

    return Parser


parser = parser_int()
print(parser("123x"))
print(parser("foo"))
