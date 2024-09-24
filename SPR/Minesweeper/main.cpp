#include <iostream>
#include <string>
#include <vector>

int getCount(const std::vector<int> &data, int row, int col, int w, int h)
{
    int count = 0;
    for (int i = -1; i <= 1; ++i)
    {
        int y = row + i;
        if (y < 0 || y >= h)
            continue;

        for (int j = -1; j <= 1; ++j)
        {
            int x = col + j;

            if (x < 0 || x >= w)
                continue;

            if (data[y * w + x] == -1)
            {
                ++count;
            }
        }
    }

    return count;
}

void solve(int w, int h)
{
    std::vector<int> data;
    data.reserve(w * h);
    std::string row;
    row.reserve(w);

    for (int i = 0; i < h; ++i)
    {
        std::cin >> row;

        for (int j = 0; j < w; ++j)
        {
            if (row[j] == '.')
            {
                data[w * i + j] = 0;
            }
            else
            {
                data[w * i + j] = -1;
            }
        }
    }

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            if (data[w * i + j] == -1)
            {
                std::cout << "*";
            }
            else
            {
                std::cout << getCount(data, i, j, w, h);
            }
        }
        std::endl(std::cout);
    }
}

int main()
{
    int w, h;

    while (true)
    {
        std::cin >> w >> h;

        if (w == 0 && h == 0)
        {
            break;
        }

        solve(w, h);
    }
    return 0;
}
