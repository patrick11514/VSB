#include <string>
#include <iostream>
#include "class.h"

using namespace std;

Example::Example()
{
    this->test = "Hello World";
}

void Example::hello()
{
    cout << this->test << endl;
}