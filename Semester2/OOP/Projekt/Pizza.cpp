#include "Pizza.h"
#include "Dough.h"
#include "Sauce.h"

#include <string>
#include <iostream>

int Pizza::count = 0;

Pizza::Pizza(std::string name, int maxIngredientsCount)
{
    Pizza::count++;
    this->name = name;
    this->maxIngredientsCount = maxIngredientsCount;
    this->ingredientsCount = 0;
    this->ingredients = new Ingredient *[maxIngredientsCount];
    this->price = 0;

    this->addDefaultIngredients();
}

Pizza::~Pizza()
{
    for (int i = 0; i < this->ingredientsCount; i++)
    {
        delete this->ingredients[i];
    }
    delete[] this->ingredients;

    Pizza::count--;
}

int Pizza::getClassCount()
{
    return Pizza::count;
}

std::string Pizza::getName()
{
    return this->name;
}

int Pizza::getIngredientsCount()
{
    return this->ingredientsCount;
}

Ingredient *Pizza::getIngredient(int index)
{
    if (index < 0 || index >= this->ingredientsCount)
    {
        return nullptr;
    }

    return this->ingredients[index];
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

void Pizza::removeIngredient(int index)
{
    if (index < 0 || index >= this->ingredientsCount)
    {
        return;
    }

    delete this->ingredients[index];
    this->ingredients[index] = nullptr;
    this->ingredientsCount--;

    for (int i = index; i < this->ingredientsCount; i++)
    {
        this->ingredients[i] = this->ingredients[i + 1];
    }
}

void Pizza::addDefaultIngredients()
{
    this->addDough(1);
    this->addSauce(1);
}

void Pizza::setPrice(int price)
{
    this->price = price;
}

int Pizza::getPrice()
{
    return this->price;
}