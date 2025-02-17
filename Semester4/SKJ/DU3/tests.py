from tasks import count_successive, find_positions, invert_dictionary, is_palindrome, \
    lex_compare


def test_lex_compare():
    assert lex_compare('a', 'b') == 'a'
    assert lex_compare('ahoj', 'buvol') == 'ahoj'
    assert lex_compare('ahoj', 'ahojky') == 'ahoj'
    assert lex_compare('dum', 'automobil') == 'automobil'
    assert lex_compare('', '') == ''
    assert lex_compare('abc', 'abd') == 'abc'
    assert lex_compare('dbe', 'dca') == 'dbe'


def test_is_palindrome():
    assert is_palindrome('')
    assert is_palindrome('a')
    assert is_palindrome('aa')
    assert is_palindrome('aba')
    assert is_palindrome('bbbb')
    assert not is_palindrome('ab')
    assert not is_palindrome('abc')
    assert not is_palindrome('abca')


def test_count_successive():
    assert count_successive("") == []
    assert count_successive("aaabbcccc") == [("a", 3), ("b", 2), ("c", 4)]
    assert count_successive("aabaaa") == [("a", 2), ("b", 1), ("a", 3)]
    assert count_successive("aaaa") == [("a", 4)]


def test_find_positions():
    assert find_positions([]) == {}
    assert find_positions([0, 0, 0]) == {0: [0, 1, 2]}
    assert find_positions(["hello", 1, 1, 2, "hello", 2]) == {
        2: [3, 5],
        "hello": [0, 4],
        1: [1, 2]
    }


def test_invert_dictionary():
    assert invert_dictionary({}) == {}
    assert invert_dictionary({1: 2, 3: 4}) == {2: 1, 4: 3}
    assert invert_dictionary({1: 2, 3: 2}) is None
    assert invert_dictionary({1: 2, 3: "2"}) == {2: 1, "2": 3}
    assert invert_dictionary({1: 2, 3: 4, "a": "b", 4: 3}) == {2: 1, 4: 3, 3: 4, "b": "a"}
