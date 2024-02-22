"""
Script for the first laboratory.
Add code by following instructions of the teacher.
"""


def add(a: int, b: int) -> int:
    """Adds parameters."""
    return a + b


def what_number(number: int) -> str:
    """Returns string positive/zero/negative specifying
    value of the number."""
    # if <expr>:
    # elif <expr>:
    # else:

    if number > 0:
        return "positive"
    elif number < 0:
        return "negative"
    return "number"


def sum_of_numbers(numbers: list[int]) -> int:
    """Returns sum of the numbers in the list."""
    sum = 0
    for i in numbers:
        sum += i
    return sum


def ship_name(fleet: str, designated_no: dict[str, str]) -> str | None:
    """Return ship's name for specified designated number
    from the fleet."""
    # has dictionary a key? Use syntax: key in dictionary
    if designated_no in fleet:
        return fleet[designated_no]
    return None


def how_many(numbers: list[int], treshold: int) -> int:
    """Returns number of numbers greater than 5."""
    # Modify example to take argument that specifies threshold
    count = 0
    for i in numbers:
        if i > treshold:
            count += 1
    return count


def gen_list_gt(lst: list[int], no: int) -> list[int]:
    """Returns list with numbers greater than no."""
    # syntax: [ item for item in lst if_condition ]
    return [item for item in lst if item > no]


print(add(1, 3))
# print(add([1, 2, 3], [4, 5, 6]))
# Try addition of strings or different data type and see what happens

# if statement example
n = 5
print("Number", n, "is:", what_number(n))

# for example: sum of the list example
lst = [1, 2, 3, 6, 7, 8]
print("Sum is:", sum_of_numbers(lst))

# dictionary example
fleet = {"BS62": "Pegasus", "BS75": "Galactica", 36: "Valkirie"}
designated_no = "BS62"
print("We've got {} in the fleet".format(ship_name(fleet, designated_no)))

# function to count how many numbers > 5 are in the list
lst = [1, 2, 5, 6, 7, 10, 12, 40, 3]
print("There are {} numbers greater than 5".format(how_many(lst)))

# generating list example
lst = [1, 2, 3, 4, 5, 6, 7, 8, 9]
no = 5
print("List with numbers > {}: {}".format(no, gen_list_gt(lst, no)))
