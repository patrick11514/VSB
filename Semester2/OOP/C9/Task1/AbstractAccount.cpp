#include "AbstractAccount.h"

#include <iostream>

AbstractAccount::AbstractAccount()
{
    std::cout << "Abstract Constructor" << std::endl;
}

AbstractAccount::~AbstractAccount()
{
    std::cout << "Abstract destructor" << std::endl;
}
