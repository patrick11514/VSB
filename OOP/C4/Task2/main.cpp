#include <iostream>
#include <string>
#include "class.h"

int main()
{

    Example *example = new Example();
    example->hello();
    delete example;
    return 0;
}