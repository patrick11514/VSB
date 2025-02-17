#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

void increase(std::vector<int> &data, int x, int y, int w, int h)
{
    for (int i = -1; i <= 1; ++i)
    {
        int row = y + i;

        if (row < 0 || row >= h)
        {
            continue;
        }

        for (int l = -1; l <= 1; ++l)
        {
            int col = x + l;

            if (col < 0 || col >= w)
            {
                continue;
            }

            int index = row * w + col;

            if (row == y && col == x)
            {
                data[index] = -1;
                continue;
            }

            if (data[index] >= 0)
            {
                data[index]++;
            }
        }
    }
}

void solve(int w, int h)
{
    std::vector<int> data(w * h, 0);

    for (int y = 0; y < h; ++y)
    {
        std::string row;
        row.reserve(w);
        std::cin >> row;

        for (int x = 0; x < w; ++x)
        {
            if (row[x] != '*')
            {
                continue;
            }

            increase(data, x, y, w, h);
        }
    }

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            int n = data[y * w + x];
            if (n == -1)
            {
                std::cout << "*";
            }
            else
            {
                std::cout << n;
            }
        }
        std::endl(std::cout);
    }
}

int main()
{
    int w, h, count = 1;

    while (true)
    {
        std::cin >> h >> w;

        if (w == 0 && h == 0)
        {
            break;
        }

        if (count > 1)
        {
            std::endl(std::cout);
        }

        std::cout << "Field #" << count << ":" << std::endl;
        solve(w, h);

        ++count;
    }
    return 0;
}
