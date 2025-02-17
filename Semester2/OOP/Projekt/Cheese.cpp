#include "Cheese.h"

#include <string>

Cheese::Cheese(int quantity, std::string type) : Ingredient("Cheese - " + type, quantity)
{
    this->type = type;
}

Cheese::~Cheese()
{
}

std::string Cheese::getType()
{
    return this->type;
}

void Cheese::setType(std::string type)
{
    this->type = type;
}
