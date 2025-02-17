#include <iostream>
#include <vector>
using namespace std;

void print_res(vector<int> v)
{
    int s = v.size();
    int i = 0;
    if (s >= 1)
        cout << v[i];
    for (i = 1; i < s; i++)
    {
        cout << " " << v[i];
    }
    cout << endl;
}

void factorization(int n, vector<int> &v)
{
    for (int y = 2; y <= n; y++)
    {
        while (n % y == 0)
        {
            v.push_back(y);
            n = n / y;
        }
    }
}

int main()
{
    int count;
    cin >> count;
    for (int i = 0; i < count; i++)
    {
        int x;
        cin >> x;
        vector<int> result;
        factorization(x, result);
        print_res(result);
    }
    return 0;
}