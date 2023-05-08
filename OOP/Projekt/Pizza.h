#pragma once

#include <string>

#include "Ingredient.h"

class Pizza
{
private:
    std::string name;
    Ingredient **ingredients;
    int ingredientsCount;
    int maxIngredientsCount;

public:
    Pizza(std::string name, int maxIngredientsCount);
    ~Pizza();

    void addDough(int quantity);
    void addSauce(int quantity);
};
