#include "Ingredient.h"

#include <string>

Ingredient::Ingredient(std::string name, int quantity)
{
    this->name = name;
    this->quantity = quantity;
}

Ingredient::~Ingredient()
{
}

std::string Ingredient::getName()
{
    return this->name;
}

int Ingredient::getQuantity()
{
    return this->quantity;
}

void Ingredient::setName(std::string name)
{
    this->name = name;
}

void Ingredient::setQuantity(int quantity)
{
    this->quantity = quantity;
}
