from tasks import (
    bonus_fizzbuzz,
    bonus_utf8,
    count_words,
    dot_product,
    fibonacci,
    fizzbuzz,
    redact,
)


def test_fizzbuzz():
    assert fizzbuzz(3) == "Fizz"
    assert fizzbuzz(5) == "Buzz"
    assert fizzbuzz(15) == "FizzBuzz"
    assert fizzbuzz(25) == "Buzz"
    assert fizzbuzz(8) == 8
    assert fizzbuzz(45) == "FizzBuzz"
    assert fizzbuzz(1) == 1
    assert fizzbuzz(0) == "FizzBuzz"


def test_fibonacci():
    assert fibonacci(0) == 0
    assert fibonacci(1) == 1
    assert fibonacci(2) == 1
    assert fibonacci(3) == 2
    assert fibonacci(4) == 3
    assert fibonacci(5) == 5
    assert fibonacci(6) == 8
    assert fibonacci(7) == 13
    assert fibonacci(10) == 55
    assert fibonacci(20) == 6765
    assert fibonacci(30) == 832040


def test_dot_product():
    assert dot_product([1, 2, 3], [0, 3, 4]) == 18
    assert dot_product((-1,), (5,)) == -5
    assert "{:.2f}".format(dot_product((-5.6, 0, 10), (12, 8, 5))) == "-17.20"


def test_redact():
    assert redact("Hello world!", "lo") == "Hexxx wxrxd!"
    assert redact("Secret message", "mse") == "Sxcrxt xxxxagx"
    assert redact("No spaces allowed", " ") == "Noxspacesxallowed"
    assert redact("S", "s") == "S"
    assert redact("xxsx", "s") == "xxxx"
    assert redact("", []) == ""


def test_count_words():
    assert count_words("hello is this the crusty crab no this is patrick") == {
        "hello": 1,
        "is": 2,
        "this": 2,
        "the": 1,
        "crusty": 1,
        "crab": 1,
        "no": 1,
        "patrick": 1,
    }
    assert count_words("what happens in kernel mode stays in kernel mode") == {
        "what": 1,
        "happens": 1,
        "in": 2,
        "kernel": 2,
        "mode": 2,
        "stays": 1,
    }
    assert count_words("") == {}


def test_bonus_fizzbuzz():
    assert bonus_fizzbuzz(3) == "Fizz"
    assert bonus_fizzbuzz(5) == "Buzz"
    assert bonus_fizzbuzz(6) == "Fizz"
    assert bonus_fizzbuzz(15) == "FizzBuzz"
    assert bonus_fizzbuzz(25) == "Buzz"
    assert bonus_fizzbuzz(8) == 8
    assert bonus_fizzbuzz(45) == "FizzBuzz"
    assert bonus_fizzbuzz(1) == 1
    assert bonus_fizzbuzz(0) == "FizzBuzz"


def test_bonus_utf8():
    assert bytes(bonus_utf8(ord("0"))).decode("utf-8") == "0"
    assert bytes(bonus_utf8(ord("č"))).decode("utf-8") == "č"
    assert bytes(bonus_utf8(ord("⤴"))).decode("utf-8") == "⤴"
    assert bytes(bonus_utf8(ord("😁"))).decode("utf-8") == "😁"
