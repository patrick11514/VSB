def fizzbuzz(num: int):
    """
    Return 'Fizz' if `num` is divisible by 3, 'Buzz' if `num` is divisible by 5, 'FizzBuzz' if `num` is divisible both by 3 and 5.
    If `num` isn't divisible neither by 3 nor by 5, return `num`.
    Example:
        fizzbuzz(3) # Fizz
        fizzbuzz(5) # Buzz
        fizzbuzz(15) # FizzBuzz
        fizzbuzz(8) # 8
    """

    if num % 3 == 0 and num % 5 == 0:
        return "FizzBuzz"
    elif num % 3 == 0:
        return "Fizz"
    elif num % 5 == 0:
        return "Buzz"
    return num


def fibonacci(n: int):
    prev = 0
    current = 1

    for _ in range(0, n):
        temp = current
        current = prev + current
        prev = temp

    return prev


def dot_product(a: list[int], b: list[int]):
    """
    Calculate the dot product of `a` and `b`.
    Assume that `a` and `b` have same length.
    Hint:
        lookup `zip` function
    Example:
        dot_product([1, 2, 3], [0, 3, 4]) == 1*0 + 2*3 + 3*4 == 18
    """
    sum = 0
    for x, y in zip(a, b):
        sum += x * y

    return sum


def redact(data: str, chars: str):
    """
    Return `data` with all characters from `chars` replaced by the character 'x'.
    Characters are case sensitive.
    Example:
        redact("Hello world!", "lo")        # Hexxx wxrxd!
        redact("Secret message", "mse")     # Sxcrxt xxxxagx
    """
    new = data
    for c in chars:
        new = new.replace(c, "x")

    return new


def count_words(data: str):
    """
    Return a dictionary that maps word -> number of occurences in `data`.
    Words are separated by spaces (' ').
    Characters are case sensitive.

    Hint:
        "hi there".split(" ") -> ["hi", "there"]

    Example:
        count_words('this car is my favourite what car is this')
        {
            'this': 2,
            'car': 2,
            'is': 2,
            'my': 1,
            'favourite': 1,
            'what': 1
        }
    """
    dict = {}

    for word in data.split(" "):
        if word == "":
            continue
        if not word in dict:
            dict[word] = 0
        dict[word] += 1

    return dict


def bonus_fizzbuzz(num: int) -> str | int:
    """
    Implement the `fizzbuzz` function.
    `if`, match-case and cycles are not allowed.

    Example:
    fizzbuzz(3) # Fizz
    fizzbuzz(5) # Buzz
    fizzbuzz(15) # FizzBuzz
    fizzbuzz(8) # 8

    """
    dict = {0: num, 1: "Fizz", 2: "Buzz", 3: "FizzBuzz"}

    return dict[int(not bool(num % 3)) + (int(not bool(num % 5)) * 2)]


def bonus_utf8(cp: int):
    """
    Encode `cp` (a Unicode code point) into 1-4 UTF-8 bytes - you should know this from `Základy číslicových systémů (ZDS)`.
    Example:
        bonus_utf8(0x01) == [0x01]
        bonus_utf8(0x1F601) == [0xF0, 0x9F, 0x98, 0x81]
    """

    # https://en.wikipedia.org/wiki/UTF-8#Encoding

    # U+0000 - U+007F
    # handle first case
    if cp <= 0x7F:
        return [cp]

    # handle other cases
    parts = 0

    def create_mask(ones: int):
        base = 1
        for _ in range(7):
            base <<= 1
            # or shifted base with one if ones > 0 or zero
            base |= int(bool(ones))
            if ones > 0:
                ones -= 1
        return base

    if cp <= 0x7FF:
        parts = 1
    elif cp <= 0xFFFF:
        parts = 2
    else:
        parts = 3

    list = []

    # extract first part
    first = cp >> (6 * parts)
    # add prefix to part with function, that creates mask
    # and put it in the list
    list.append(first | create_mask(parts))

    # extract other parts
    for i in range(parts):
        # counts bacwards => parts = 2
        # first itteration 2, second 1, third, 0
        shifter = parts - i - 1

        # shift cp with parts * 6 to get specific part
        # and then and with 0b111111 to remove other bits
        shifted = (cp >> shifter * 6) & 0b111111
        # now add prefix 0b10 to bits and put in the list
        list.append(shifted | 0b10000000)

    return list
