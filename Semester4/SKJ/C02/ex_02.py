def envelope(string: str) -> str:
    return f"!{string}!"


def use_envelope(lst: list[str]) -> list[str]:
    return list(map(envelope, lst))


def super_print(*arg: str):
    """
    Prints string of joined strings from *arg separated by whitespace.
    On the top and bottom of the string, `-` character is print in the same length
    as the output string.

    Example:
      super_print("Hello", "World!")

      ------------
      Hello World!
      ------------
    """

    if len(arg) == 0:
        return

    string = " ".join(arg)

    strlen = len(string)

    print("-" * strlen)
    print(string)
    print("-" * strlen)


super_print("Aa", "bb", "cc", "dd", "!")
