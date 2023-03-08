#include <iostream>
#include <string>

using namespace std;

class Example
{
private:
    string test = "Hello World";

public:
    Example()
    {
    }
    void hello()
    {
        cout << test << endl;
    }
};

int main()
{

    Example *example = new Example();
    example->hello();
    delete example;
    return 0;
}