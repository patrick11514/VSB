#include "Vegetarian.h"
#include "Pizza.h"
#include "Vegetables.h"

Vegetarian::Vegetarian() : Pizza("Vegetarian", 3)
{
    this->setPrice(130);
}

Vegetarian::~Vegetarian()
{
}

void Vegetarian::addDefaultIngredients()
{
    this->addDough(1);
    this->addSauce(1);
    this->addVegetables(1);
}

void Vegetarian::addVegetables(int quantity)
{
    Vegetables *vegetables = new Vegetables(quantity);
    this->ingredients[this->ingredientsCount++] = vegetables;
}