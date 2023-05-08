#include "Ingredient.h"

#include <string>

Ingredient::Ingredient(std::string, int quantity)
{
    this->name = name;
    this->quantity = quantity;
}

Ingredient::~Ingredient()
{
}

int Ingredient::getName()
{
    return this->name;
}

int Ingredient::getQuantity()
{
    return this->quantity;
}

void Ingredient::setName(int name)
{
    this->name = name;
}

void Ingredient::setQuantity(int quantity)
{
    this->quantity = quantity;
}
