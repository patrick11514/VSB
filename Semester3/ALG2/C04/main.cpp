#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <algorithm>

void readFile(std::string fileName, std::set<int> &s)
{
    std::ifstream file(fileName);

    if (!file)
    {
        std::cout << "File " << fileName << " not found" << std::endl;
        return;
    }

    while (!file.eof())
    {
        int x;
        file >> x;
        s.insert(x);
    }

    file.close();
}

void findIntersection(std::set<int> &a, std::set<int> &b, std::string file)
{
    std::ofstream outFile(file);

    for (std::set<int>::iterator i = a.begin(); i != a.end(); ++i)
    {
        int value = *i;
        if (b.find(value) != b.end())
        {
            outFile << value << "\n";
        }
    }

    outFile.close();
}

int main()
{
    std::set<int> a;
    readFile("Mnozina1.txt", a);

    std::set<int> b;
    readFile("Mnozina2.txt", b);

    findIntersection(a, b, "out.txt");

    return 0;
}
