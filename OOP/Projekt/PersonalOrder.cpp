#include "PersonalOrder.h"

PersonalOrder::PersonalOrder(std::string name, std::string address, std::string phone, int maxPizzasCount) : Order(name, address, phone, maxPizzasCount)
{
}

PersonalOrder::~PersonalOrder()
{
}
