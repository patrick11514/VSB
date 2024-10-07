#include <iostream>
#include <string>
#include <vector>

enum Orientation
{
    ROW,
    COL
};

struct Piece
{
    Orientation orientation;

    Piece(Orientation orientation)
    {
        this->orientation = orientation;
    }

    void swap()
    {
        if (this->orientation == Orientation::ROW)
        {
            this->orientation = Orientation::COL;
        }
        else
        {
            this->orientation = Orientation::ROW;
        }
    }

    char getChar() const
    {
        if (this->orientation == Orientation::ROW)
        {
            return '_';
        }
        return '|';
    }
};

void print(const std::vector<std::vector<Piece>> &layout)
{
    for (const auto &row : layout)
    {
        for (const auto &item : row)
        {
            std::cout << item.getChar();
        }

        std::endl(std::cout);
    }
}

void addRotation(std::vector<std::vector<Piece>> &layout, int rotX, int rotY)
{
    std::vector<std::vector<Piece>> copy{layout};
}

void makeFolds(int folds, int iterations, std::vector<std::vector<Piece>> &layout, int width, int height)
{
    if (folds == 0)
    {
        print(layout);
        std::cout << "^" << std::endl;
        return;
    }

    if (layout.size() == 0)
    {
        layout.push_back({Piece{Orientation::ROW}, Piece{Orientation::COL}});
        return makeFolds(folds - 1, 2, layout, 2, 1);
    }

    std::vector<std::vector<Piece>> rotated;

    for (int col = 0; col < layout[0].size(); ++col)
    {
        std::vector<Piece> newRow;
        for (size_t row = 0; row < layout.size(); ++row)
        {
            auto item = layout[row][col];
            item.swap();
            newRow.push_back(item);
        }
        rotated.push_back(newRow);
    }

    std::cout << "NORMAL: " << std::endl;
    print(layout);
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
