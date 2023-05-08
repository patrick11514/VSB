#pragma once

#include "Ingredient.h"

#include <string>

class Cheese : public Ingredient
{
private:
    std::string type;

public:
    Cheese(int quantity, std::string type);
    ~Cheese();

    std::string getType();

    void setType(std::string type);
};
