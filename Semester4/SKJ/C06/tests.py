import pytest

from tasks import GameOfLife, cached


def test_cached_simple():
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


def test_cached_complex():
    counts = 0

    @cached
    def counter(*a):
        nonlocal counts
        print(f"before {counts}")
        counts += sum(a)
        print(f"after {counts}")
        return sum(a)

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
    assert counter(0, 1) == 1
    assert counts == 14

    counts = 0
    assert counter(0, 1, 0, 1, 0, 1, 0) == 3
    assert counts == 3
    assert counter(0, 1, 0, 1, 0, 1, 1, 0) == 4
    assert counts == 7
    assert counter(0, 1, 0, 1, 0, 1, 2) == 5
    assert counts == 12
    assert counter(0, 1, 0, 1, 0, 1, 0) == 3
    assert counts == 12
    assert counter(0, 1, 0, 1, 0, 1, 3) == 6
    assert counts == 18
    assert counter(0, 1, 0, 1, 0, 1, 1, 0) == 4
    assert counts == 22


def test_game_of_life_count_print():
    g = GameOfLife(((".", ".", "."), (".", ".", "."), (".", ".", ".")))
    assert g.alive() == 0
    assert g.dead() == 9
    assert str(g) == "...\n...\n...\n"

    g = GameOfLife((("x", ".", "."), (".", "x", "."), (".", "x", "x")))
    assert g.alive() == 4
    assert g.dead() == 5
    assert str(g) == "x..\n.x.\n.xx\n"


def test_game_of_life_move():
    assert GameOfLife(
        (
            (".", ".", "."),
            (".", "x", "."),
            (".", "x", "."),
            (".", "x", "."),
            (".", ".", "."),
        )
    ).move().board == (
        (".", ".", "."),
        (".", ".", "."),
        ("x", "x", "x"),
        (".", ".", "."),
        (".", ".", "."),
    )

    assert GameOfLife(
        (
            (".", ".", ".", "."),
            (".", "x", "x", "."),
            (".", "x", "x", "."),
            (".", ".", ".", "."),
        )
    ).move().board == (
        (".", ".", ".", "."),
        (".", "x", "x", "."),
        (".", "x", "x", "."),
        (".", ".", ".", "."),
    )

    assert GameOfLife(
        (
            ("x", ".", "."),
            (".", "x", "x"),
            ("x", "x", "."),
        )
    ).move().board == (
        (".", "x", "."),
        (".", ".", "x"),
        ("x", "x", "x"),
    )


def test_game_of_life_move_immutable():
    board = (
        (".", ".", "."),
        (".", "x", "."),
        (".", "x", "."),
        (".", "x", "."),
        (".", ".", "."),
    )

    g = GameOfLife(board)
    n = g.move()
    assert id(n.board) != id(board)
    assert board == (
        (".", ".", "."),
        (".", "x", "."),
        (".", "x", "."),
        (".", "x", "."),
        (".", ".", "."),
    )
    assert n.board == (
        (".", ".", "."),
        (".", ".", "."),
        ("x", "x", "x"),
        (".", ".", "."),
        (".", ".", "."),
    )
