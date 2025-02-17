#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using Board = std::vector<std::string>;

const std::unordered_map<char, std::vector<std::tuple<int, int>>> MOVES{
    {'P', {{-1, -1}, {1, -1}}},
    {'p', {{-1, 1}, {1, 1}}},
    {'k', {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}},
    {'K', {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}},
    {'n', {
              {-2, -1},
              {-1, -2},
              {-1, 2},
              {-2, 1},
              {1, 2},
              {2, 1},
              {1, -2},
              {2, -1},
          }},
    {'N', {
              {-2, -1},
              {-1, -2},
              {-1, 2},
              {-2, 1},
              {1, 2},
              {2, 1},
              {1, -2},
              {2, -1},
          }},
};

const int WIDTH = 8;
const int HEIGHT = 8;

struct Point
{
    bool found = false;
    int x = 0;
    int y = 0;
};

int checkKing(char c, int x, int y, Point &blackKing, Point &whiteKing, Board &board)
{
    if (blackKing.found == true && blackKing.x == x && blackKing.y == y && (c >= 'A' && c <= 'Z'))
    {
        return 1;
    }

    if (whiteKing.found == true && whiteKing.x == x && whiteKing.y == y && (c >= 'a' && c <= 'z'))
    {
        return 2;
    }

    if (y < HEIGHT && y >= 0 && x < WIDTH && x >= 0 && board[y][x] != '.')
    {
        return -1;
    }

    return 0;
}

void findKings(Point &blackKing, Point &whiteKing, Board &board)
{
    for (size_t rows = 0; rows < board.size(); ++rows)
    {
        const auto &row = board[rows];
        for (size_t cols = 0; cols < row.size(); ++cols)
        {
            char c = row[cols];

            if (c == 'k')
            {
                blackKing.found = true;
                blackKing.x = cols;
                blackKing.y = rows;
            }
            else if (c == 'K')
            {
                whiteKing.found = true;
                whiteKing.x = cols;
                whiteKing.y = rows;
            }
        }
    }
}

// 0 = no one, 1 = black, 2 = white
int check(Point &blackKing, Point &whiteKing, Board &board)
{
    for (size_t rows = 0; rows < board.size(); ++rows)
    {
        const auto &row = board[rows];
        for (size_t cols = 0; cols < row.size(); ++cols)
        {
            char c = row[cols];

            if (c == 'r' || c == 'R' || c == 'q' || c == 'Q')
            {
                // horizontal
                bool plus = true;
                bool minus = true;

                for (int x = 1; x < WIDTH; ++x)
                {
                    if (plus)
                    {
                        int result = checkKing(c, cols + x, rows, blackKing, whiteKing, board);
                        if (result == -1)
                        {
                            plus = false;
                        }
                        if (result > 0)
                            return result;
                    }

                    if (minus)
                    {
                        int result = checkKing(c, cols - x, rows, blackKing, whiteKing, board);
                        if (result == -1)
                        {
                            minus = false;
                        }
                        if (result > 0)
                            return result;
                    }
                }

                // vertical
                minus = plus = true;
                for (int y = 1; y < HEIGHT; ++y)
                {
                    if (plus)
                    {
                        int result = checkKing(c, cols, rows + y, blackKing, whiteKing, board);
                        if (result == -1)
                        {
                            plus = false;
                        }
                        if (result > 0)
                            return result;
                    }

                    if (minus)
                    {
                        int result = checkKing(c, cols, rows - y, blackKing, whiteKing, board);
                        if (result == -1)
                        {
                            minus = false;
                        }
                        if (result > 0)
                            return result;
                    }
                }
            }

            if (c == 'q' || c == 'Q' || c == 'b' || c == 'B')
            {
                bool plus = true;
                bool minus = true;
                // u1
                for (int inc = 1; inc < WIDTH; ++inc)
                {
                    if (plus)
                    {
                        int result = checkKing(c, cols + inc, rows + inc, blackKing, whiteKing, board);

                        if (result == -1)
                        {
                            plus = false;
                        }
                        if (result > 0)
                            return result;
                    }
                    if (minus)
                    {
                        int result = checkKing(c, cols - inc, rows - inc, blackKing, whiteKing, board);

                        if (result == -1)
                        {
                            minus = false;
                        }
                        if (result > 0)
                            return result;
                    }
                }
                // u2
                plus = minus = true;
                for (int inc = 1; inc < WIDTH; ++inc)
                {
                    if (plus)
                    {
                        int result = checkKing(c, cols + inc, rows - inc, blackKing, whiteKing, board);

                        if (result == -1)
                        {
                            plus = false;
                        }
                        if (result > 0)
                            return result;
                    }

                    if (minus)
                    {
                        int result = checkKing(c, cols - inc, rows + inc, blackKing, whiteKing, board);

                        if (result == -1)
                        {
                            minus = false;
                        }
                        if (result > 0)
                            return result;
                    }
                }
            }

            auto it = MOVES.find(c);
            if (it == MOVES.end())
            {
                continue;
            }

            for (const auto &coords : it->second)
            {
                int result = checkKing(c, std::get<0>(coords) + cols, std::get<1>(coords) + rows, blackKing, whiteKing, board);

                // don't check if result is == -1, because we want to check all point pairs
                if (result > 0)
                    return result;
            }
        }
    }

    return 0;
}

int main()
{
    int game = 1;
    while (true)
    {
        Board board;
        board.reserve(WIDTH);

        for (int i = 0; i < 8; ++i)
        {
            std::string row;
            std::cin >> row;
            board.emplace_back(row);
        }

        Point blackKing;
        Point whiteKing;

        findKings(blackKing, whiteKing, board);

        if (!blackKing.found && !whiteKing.found)
        {
            break;
        }

        int result = check(blackKing, whiteKing, board);

        std::cout << "Game #" << game << ": ";
        switch (result)
        {
        case 0:
            std::cout << "no king is in check.";
            break;
        case 1:
            std::cout << "black king is in check.";
            break;
        case 2:
            std::cout << "white king is in check.";
        default:
            break;
        }

        std::endl(std::cout);

        ++game;
    }
    return 0;
}
