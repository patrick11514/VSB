def read_file(filename: str) -> str:
    """
    Returns content of a file as a string.

    Uses classic file access.
    """

    f = None
    lines = "Unable to open a file: {0}".format(filename)
    try:
        f = open(filename)
        lines = f.read()
    except FileNotFoundError:
        lines = "File not found"
    finally:
        return lines


def read_file_using_with(filename: str) -> str:
    """
    Returns content of a file as a string.

    Uses with statement to open a file.
    """

    lines = "Unable to open a file: {0}".format(filename)
    try:
        with open(filename) as file:
            lines = file.read()
    except FileNotFoundError:
        lines = "File not found"
    finally:
        return lines


def filter_cities(filename: str, **kw: int) -> list[str]:
    """
    Returns a list of cities that match their population specified by restrictions.
    In **kw it will be possible to set an argument:
     'gt': only cities with a population greater than the value of the argument
     'lt': only cities with a population less than the value of the argument
    It is possible to enter none, one or both parameters at once.
    If no parameter is specified, an empty list is returned.
    Use list comprehension for filtering.
    Use so-called "unpacking" to load data.

    Reimplement the function to return a generator.
    """

    if len(kw) == 0:
        return []

    cities = list(map(lambda x: x.split(":"), read_file(filename).splitlines()))
    filtered = cities

    if "gt" in kw:
        filtered = [[name, value] for name, value in filtered if kw["gt"] <= int(value)]

    if "lt" in kw:
        filtered = [[name, value] for name, value in filtered if kw["lt"] >= int(value)]

    return list(map(lambda x: x[0], filtered))


def filter_cities_gen(filename: str, **kw: int):
    """
    Returns a list of cities that match their population specified by restrictions.
    In **kw it will be possible to set an argument:
     'gt': only cities with a population greater than the value of the argument
     'lt': only cities with a population less than the value of the argument
    It is possible to enter none, one or both parameters at once.
    If no parameter is specified, an empty list is returned.
    Use list comprehension for filtering.
    Use so-called "unpacking" to load data.

    Reimplement the function to return a generator.
    """

    if len(kw) == 0:
        return []

    cities = list(map(lambda x: x.split(":"), read_file(filename).splitlines()))
    filtered = cities

    if "gt" in kw:
        filtered = [[name, value] for name, value in filtered if kw["gt"] <= int(value)]

    if "lt" in kw:
        filtered = [[name, value] for name, value in filtered if kw["lt"] >= int(value)]

    for i in map(lambda x: x[0], filtered):
        yield i


def main():
    """
    Main function.
    This function is run at the startup of a script thanks to the cryptic if statement below.
    """

    # file reading
    filename = "cities.txt"
    content = read_file(filename)
    print("{} file content is: {}".format(filename, content))

    content = read_file_using_with(filename)
    print("{} file content using with statement is: {}".format(filename, content))

    # filtering
    print(list(filter_cities(filename, gt=500000)))
    print(list(filter_cities(filename, lt=300000)))
    print(list(filter_cities(filename, lt=300000, gt=500000)))
    print(list(filter_cities_gen(filename, gt=500000)))
    print(list(filter_cities_gen(filename, lt=300000)))
    print(list(filter_cities_gen(filename, lt=300000, gt=500000)))


if __name__ == "__main__":
    main()
