import time
from typing import Callable


def cached(func: Callable):
    """
    Create a decorator that caches up to 3 function results, based on the same parameter values.

    When `f` is called with the same parameter values that are already in the cache, return the
    stored result associated with these parameter values. You can assume that `f` receives only
    positional arguments (you can ignore keyword arguments).

    When `f` is called with new parameter values, forget the oldest accessed result in the cache
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

    cache: dict[str, int] = {}
    usage_list: list[str] = []

    def wrapper(*args):
        nonlocal cache
        nonlocal usage_list

        key = ",".join(map(lambda x: str(x), args))

        if key in cache:
            # move to front
            usage_list.remove(key)
            usage_list.append(key)
            return cache[key]
        else:
            if len(cache.keys()) == 3:
                removedKey = usage_list.pop(0)
                del cache[removedKey]

            result = func(*args)
            cache[key] = result

            usage_list.append(key)
            return result

    return wrapper


class GameOfLife:
    """
    Implement "Game of life" (https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

    The game board will be represented with nested tuples, where '.'
    marks a dead cell and 'x' marks a live cell. Cells that are out of bounds of the board are
    assumed to be dead.

    Try some patterns from wikipedia + the provided tests to test the functionality.

    The GameOfLife objects should be immutable, i.e. the move method will return a new instance
    of GameOfLife.

    Example:
        game = GameOfLife((
            ('.', '.', '.'),
            ('.', 'x', '.'),
            ('.', 'x', '.'),
            ('.', 'x', '.'),
            ('.', '.', '.')
        ))
        game.alive()    # 3
        game.dead()     # 12
        x = game.move() # 'game' doesn't change
        # x.board:
        (
            ('.', '.', '.'),
            ('.', '.', '.'),
            ('x', 'x', 'x'),
            ('.', '.', '.'),
            ('.', '.', '.')
        )

        str(x)
        ...\n
        ...\n
        xxx\n
        ...\n
        ...\n
    """

    def __init__(self, board: tuple[tuple[int]]):
        """
        Create a constructor that receives the game board and stores it in an attribute called
        'board'.
        """
        self.board = board

    def _get_neigbors(self, x: int, y: int):
        count = 0

        for newY in range(y - 1, y + 2):
            for newX in range(x - 1, x + 2):
                if newY < 0 or newX < 0:
                    continue
                if newY >= len(self.board) or newX >= len(self.board[0]):
                    continue
                if newY == y and newX == x:
                    continue
                if self.board[newY][newX] == "x":
                    count += 1

        return count

    def move(self):
        """
        Simulate one iteration of the game and return a new instance of GameOfLife containing
        the new board state.
        """
        rows: list[tuple[str]] = []

        for y in range(len(self.board)):
            row = []
            for x in range(len(self.board[y])):
                char = self.board[y][x]
                neigbors = self._get_neigbors(x, y)

                if char == "x":
                    if neigbors == 2 or neigbors == 3:
                        row.append("x")
                    else:
                        row.append(".")
                else:
                    if neigbors == 3:
                        row.append("x")
                    else:
                        row.append(".")
            rows.append(tuple(row))

        return GameOfLife(tuple(rows))

    def alive(self):
        """
        Return the number of cells that are alive.
        """
        count = 0
        for row in self.board:
            for cell in row:
                if cell == "x":
                    count += 1
        return count

    def dead(self):
        """
        Return the number of cells that are dead.
        """
        count = 0
        for row in self.board:
            for cell in row:
                if cell == ".":
                    count += 1
        return count

    def __repr__(self):
        """
        Return a string that represents the state of the board in a single string (with newlines
        for each board row).
        """
        str = ""
        for row in self.board:
            for cell in row:
                str += cell
            str += "\n"

        return str


def play_game(game: GameOfLife, n):
    """
    You can use this function to render the game for n iterations
    """
    for _ in range(n):
        print(game)
        game = game.move()
        time.sleep(0.25)  # sleep to see the output


# this code will only be executed if you run `python tasks.py`
# it will not be executed when tasks.py is imported
if __name__ == "__main__":
    play_game(
        GameOfLife(
            (
                (".", ".", "."),
                (".", "x", "."),
                (".", "x", "."),
                (".", "x", "."),
                (".", ".", "."),
            )
        ),
        10,
    )
