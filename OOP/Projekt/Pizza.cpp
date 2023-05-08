#include "Pizza.h"
#include "Dough.h"
#include "Sauce.h"

#include <string>

Pizza::Pizza(std::string name, int maxIngredientsCount);
{
    this->name = name;
    this->maxIngredientsCount = maxIngredientsCount;
    this->ingredientsCount = 0;
    this->ingredients = new Ingredient *[maxIngredientsCount];
}

Pizza::~Pizza()
{
    for (int i = 0; i < this->ingredientsCount; i++)
    {
        delete this->ingredients[i];
    }
    delete[] this->ingredients;
}

void Pizza::addDough(int quantity)
{
    Dough *dough = new Dough(quantity);
    this->ingredients[this->ingredientsCount++] = dough;
}

void Pizza::addSauce(int quantity)
{
    Sauce *sauce = new Sauce(quantity);
    this->ingredients[this->ingredientsCount++] = sauce;
}
