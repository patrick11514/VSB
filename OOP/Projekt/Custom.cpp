#include "Custom.h"
#include "Pizza.h"
#include "Cheese.h"
#include "Meat.h"
#include "Vegetables.h"

Custom::Custom(std::string name, int bonusIngredientsCount) : Pizza(name, bonusIngredientsCount + 2)
{
    this->setPrice(110);
}

Custom::~Custom()
{
}

void Custom::addCheese(int quantity, std::string type)
{
    Cheese *cheese = new Cheese(quantity, type);
    this->ingredients[this->ingredientsCount++] = cheese;
    this->setPrice(this->getPrice() + 10 * quantity);
}

void Custom::addMeat(int quantity)
{
    Meat *meat = new Meat(quantity);
    this->ingredients[this->ingredientsCount++] = meat;
    this->setPrice(this->getPrice() + 20 * quantity);
}

void Custom::addVegetables(int quantity)
{
    Vegetables *vegetables = new Vegetables(quantity);
    this->ingredients[this->ingredientsCount++] = vegetables;
    this->setPrice(this->getPrice() + 8 * quantity);
}
