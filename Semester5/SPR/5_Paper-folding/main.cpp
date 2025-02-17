#include <iostream>
#include <string>
#include <vector>

enum Orientation
{
    Top,
    Right,
    Bottom,
    Left
};

struct Move
{
    Orientation direction;

    Move(Orientation direction)
    {
        this->direction = direction;
    }

    char getChar() const
    {
        switch (this->direction)
        {
        case Orientation::Bottom:
            return 'b';
        case Orientation::Left:
            return 'l';
        case Orientation::Right:
            return 'r';
        case Orientation::Top:
            return 't';
        }
    }
};

using Moves = std::vector<Move>;

void print(const Moves &moves)
{
    for (const auto &move : moves)
    {
        std::cout << move.getChar();
    }

    std::endl(std::cout);
}

void makeFolds(int folds, int iterations, Moves &moves, int width, int height)
{
    if (folds == 0)
    {
        print(moves);
        std::cout << "^" << std::endl;
        return;
    }

    if (moves.size() == 0)
    {
        moves.push_back(Move{Orientation::Right});
        moves.push_back(Move{Orientation::Top});
        return makeFolds(folds - 1, 2, moves, 2, 1);
    }

    std::vector<std::vector<Piece>> rotated;

    for (int col = 0; col < moves[0].size(); ++col)
    {
        std::vector<Piece> newRow;
        for (size_t row = 0; row < moves.size(); ++row)
        {
            auto item = moves[row][col];
            item.swap();
            newRow.push_back(item);
        }
        rotated.push_back(newRow);
    }

    std::cout << "NORMAL: " << std::endl;
    print(moves);
    std::cout << "ROTATED: " << std::endl;
    print(rotated);
    std::cout << "W: " << width << " H: " << height << std::endl;
}

int main()
{
    while (true)
    {
        int folds;
        std::cin >> folds;

        if (folds == 0)
        {
            break;
        }

        std::vector<std::vector<Piece>> layout;

        makeFolds(folds, 1, layout, 0, 0);
    }
    return 0;
}
