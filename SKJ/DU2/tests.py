import string
from typing import TypeVar

import pytest

from tasks import (
    ParseResult,
    cached,
    parser_char,
    parser_choice,
    parser_int,
    parser_map,
    parser_matches,
    parser_repeat,
    parser_seq,
    parser_string,
)


def test_cached():
    counts = 0

    @cached
    def counter(*a):
        nonlocal counts
        counts += sum(a)
        return sum(a)

    assert counter(1) == 1
    assert counter(1) == 1
    assert counter(2) == 2
    assert counts == 3
    assert counter(1) == 1
    assert counts == 3
    assert counter(2) == 2
    assert counts == 3
    assert counter(3) == 3
    assert counts == 6
    assert counter(3) == 3
    assert counts == 6
    assert counter(4) == 4
    assert counts == 10
    assert counter(1) == 1
    assert counts == 11

    counts = 0
    assert counter(1, 2) == 3
    assert counts == 3
    assert counter(1, 2) == 3
    assert counter(1, 3) == 4
    assert counts == 7
    assert counter(0, 1) == 1
    assert counts == 8
    assert counter(0, 1) == 1
    assert counts == 8
    assert counter(1, 3) == 4
    assert counts == 8
    assert counter(1, 2) == 3
    assert counts == 8
    assert counter(1, 4) == 5
    assert counts == 13
    assert counter(1, 2) == 3
    assert counts == 16


T = TypeVar("T")


def check_valid(result: ParseResult[T], value: T, rest: str):
    assert result.is_valid()
    assert result.value == value
    assert result.rest == rest


def check_invalid(result: ParseResult[T], rest: str):
    assert not result.is_valid()
    assert result.rest == rest


def test_parser_char_empty_input():
    check_invalid(parser_char("x")(""), "")


def test_parser_char_different_char():
    check_invalid(parser_char("x")("a"), "a")


def test_parser_char_same_input():
    check_valid(parser_char("x")("x"), "x", "")


def test_parser_char_more_inputs():
    check_valid(parser_char("x")("xx"), "x", "x")
    check_valid(parser_char("x")("xa"), "x", "a")


def test_parser_char_whitespace_at_start():
    check_invalid(parser_char("x")(" x"), " x")


def test_parser_char_empty_char():
    with pytest.raises(ValueError):
        parser_char("")


def test_parser_char_multiple_chars():
    with pytest.raises(ValueError):
        parser_char("xx")


def test_parser_repeat_empty():
    parser = parser_char("x")
    parser = parser_repeat(parser)
    check_valid(parser(""), [], "")


def test_parser_repeat_no_match():
    parser = parser_char("x")
    parser = parser_repeat(parser)
    check_valid(parser("aaa"), [], "aaa")


def test_parser_repeat_match_all():
    parser = parser_char("x")
    parser = parser_repeat(parser)
    check_valid(parser("xxx"), ["x", "x", "x"], "")


def test_parser_repeat_match_some():
    parser = parser_char("x")
    parser = parser_repeat(parser)
    check_valid(parser("xxab"), ["x", "x"], "ab")


def test_parser_repeat_call_multiple_times():
    parser = parser_char("x")
    parser = parser_repeat(parser)

    check_valid(parser("xxab"), ["x", "x"], "ab")
    check_valid(parser("xxab"), ["x", "x"], "ab")
    check_valid(parser("xxab"), ["x", "x"], "ab")
    check_valid(parser("xxab"), ["x", "x"], "ab")


def test_parser_seq_no_parser():
    parser = parser_seq([])
    check_valid(parser("asd"), [], "asd")


def test_parser_seq_single_parser():
    parser = parser_seq([parser_char("a")])
    check_valid(parser("a"), ["a"], "")
    check_invalid(parser("x"), "x")


def test_parser_seq_multiple_parsers():
    parser = parser_seq(
        [
            parser_char("a"),
            parser_char("h"),
            parser_char("o"),
            parser_char("j"),
        ]
    )
    check_invalid(parser(" ahoj"), " ahoj")
    check_valid(parser("ahoj"), ["a", "h", "o", "j"], "")
    check_valid(parser("ahojx"), ["a", "h", "o", "j"], "x")
    check_invalid(parser("aho"), "aho")


def test_parser_choice_no_parsers():
    parser = parser_choice([])
    check_invalid(parser("asd"), "asd")


def test_parser_choice_no_match():
    parser = parser_choice(
        [
            parser_char("a"),
            parser_char("b"),
        ]
    )
    check_invalid(parser("x"), "x")


def test_parser_choice_match_first():
    parser = parser_choice(
        [
            parser_char("a"),
            parser_char("b"),
        ]
    )
    check_valid(parser("ab"), "a", "b")


def test_parser_choice_match_last():
    parser = parser_choice(
        [
            parser_char("a"),
            parser_char("b"),
        ]
    )
    check_valid(parser("bb"), "b", "b")


def test_parser_choice_match_all():
    parser = parser_choice(
        [
            parser_char("a"),
            parser_char("a"),
        ]
    )
    check_valid(parser("a"), "a", "")


def test_parser_choice_match_partial_1():
    parser_a = parser_seq([parser_char("a"), parser_char("b")])
    parser_b = parser_seq([parser_char("a"), parser_char("c")])

    parser = parser_choice([parser_a, parser_b])

    check_valid(parser("abc"), ["a", "b"], "c")
    check_valid(parser("acb"), ["a", "c"], "b")
    check_invalid(parser("aabc"), "aabc")


def test_parser_choice_match_partial_2():
    parser_a = parser_seq([parser_char("a"), parser_char("b")])
    parser_b = parser_char("c")

    parser = parser_choice([parser_a, parser_b])

    check_invalid(parser("ac"), "ac")


def test_parser_map_no_match():
    parser = parser_char("x")
    parser = parser_map(parser, lambda x: 1)

    check_invalid(parser("ac"), "ac")


def test_parser_map_return_none():
    parser = parser_char("x")
    parser = parser_map(parser, lambda x: None)

    check_invalid(parser("xxx"), "xxx")


def test_parser_map_valid():
    parser = parser_choice([parser_char("a"), parser_char("b")])
    parser = parser_map(parser, lambda x: x == "a")

    check_invalid(parser("xxx"), "xxx")
    check_valid(parser("ax"), True, "x")
    check_valid(parser("bx"), False, "x")


def test_parser_string_empty():
    parser = parser_string("")

    check_valid(parser(""), "", "")
    check_valid(parser("ax"), "", "ax")


def test_parser_string():
    parser = parser_string("abc")

    check_invalid(parser("a"), "a")
    check_invalid(parser("ab"), "ab")
    check_valid(parser("abc"), "abc", "")
    check_valid(parser("abca"), "abc", "a")


def test_parser_matches():
    parser = parser_matches(lambda c: c == "x" or c == "y")

    check_invalid(parser("a"), "a")
    check_valid(parser("xy"), "x", "y")
    check_valid(parser("yx"), "y", "x")


def test_parser_int_invalid():
    parser = parser_int()

    check_invalid(parser("a"), "a")
    check_invalid(parser(" 1"), " 1")
    check_invalid(parser("x42"), "x42")


def test_parser_int_valid():
    parser = parser_int()

    check_valid(parser("1"), 1, "")
    check_valid(parser("42xxy"), 42, "xxy")


def test_json_basic():
    """
    You can try to add more functions, like parser_opt (optionally parses something) and build
    a full-fledged JSON parser :)
    """
    number = parser_int()
    boolean = parser_map(
        parser_choice([parser_string("true"), parser_string("false")]),
        lambda c: c == "true",
    )
    string_inner = parser_map(
        parser_repeat(
            parser_matches(
                lambda c: c in string.ascii_letters or c.isdigit() or c in (" ", "_")
            )
        ),
        lambda chars: "".join(chars),
    )
    string_complete = parser_seq([parser_char('"'), string_inner, parser_char('"')])
    string_complete = parser_map(string_complete, lambda s: s[1])  # skip " "
    value = parser_choice([number, boolean, string_complete])

    check_invalid(value("t"), "t")
    check_invalid(value("tru"), "tru")
    check_valid(value("true"), True, "")
    check_valid(value("false"), False, "")

    check_valid(value("123"), 123, "")

    check_invalid(value('"'), '"')
    check_valid(value('""x'), "", "x")
    check_valid(value('"true"x'), "true", "x")
    check_valid(value('"123"x'), "123", "x")
    check_valid(value('"hello_123 foo"x'), "hello_123 foo", "x")
