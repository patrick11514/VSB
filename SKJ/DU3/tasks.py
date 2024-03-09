import math


def is_palindrome(data: str):
    """
    Returns True if `data` is a palindrome and False otherwise.
    Hint:
        slicing is your friend, use it
    Example:
        is_palindrome('aba') == True
        is_palindrome('abc') == False
    """

    for i in range(math.ceil(len(data) / 2)):
        if data[i] != data[::-1][i]:
            return False

    return True


def lex_compare(a: str, b: str):
    """
    Lexicographically compare `a` with `b` and return the smaller string.
    Implement the comparison yourself, do not use the `<` operator for comparing strings :)

    Example:
        lex_compare('a', 'b') == 'a'
        lex_compare('ahoj', 'buvol') == 'ahoj'
        lex_compare('ahoj', 'ahojky') == 'ahoj'
        lex_compare('dum', 'automobil') == 'automobil'
    """

    lena = len(a)
    lenb = len(b)
    length = lena if lena < lenb else lenb

    for i in range(length):
        chara = ord(a[i])
        charb = ord(b[i])

        if chara < charb:
            return a
        elif chara > charb:
            return b

    if lena < lenb:
        return a
    return b


def count_successive(string: str):
    """
    Go through the string and for each character, count how many times it appears in succession.
    Store the character and the count in a tuple and return a list of such tuples.

    Example:
          count_successive("aaabbcccc") == [("a", 3), ("b", 2), ("c", 4)]
          count_successive("aba") == [("a", 1), ("b", 1), ("a", 1)]
    """
    lst: list[tuple[str, int]] = []

    lastCount = 0
    lastChar = ""

    for i in range(len(string)):
        char = string[i]
        if char == lastChar:
            lastCount += 1
            continue

        if lastCount > 0:
            lst.append((lastChar, lastCount))

        lastCount = 1
        lastChar = char

    if lastCount > 0:
        lst.append((lastChar, lastCount))

    return lst


def find_positions(items: list[any]):
    """
    Go through the input list of items and collect indices of each individual item.
    Return a dictionary where the key will be an item and its value will be a list of indices
    where the item was found.

    Example:
        find_positions(["hello", 1, 1, 2, "hello", 2]) == {
            2: [3, 5],
            "hello": [0, 4],
            1: [1, 2]
        }
    """

    dictionary: dict[any, list[int]] = dict()

    i = 0
    for item in items:
        if item in dictionary:
            dictionary[item].append(i)
        else:
            dictionary[item] = [i]

        i += 1

    return dictionary


def invert_dictionary(dictionary: dict[any, any]):
    """
    Invert the input dictionary. Turn keys into values and vice-versa.
    If more values would belong to the same key, return None.

    Example:
        invert_dictionary({1: 2, 3: 4}) == {2: 1, 4: 3}
        invert_dictionary({1: 2, 3: 2}) is None
    """
    newDict = dict()

    for key, value in dictionary.items():
        if value in newDict:
            return None

        newDict[value] = key
    return newDict
