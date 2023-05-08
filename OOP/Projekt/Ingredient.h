#pragma once

#include <string>

class Ingredient
{
private:
    int name;
    int quantity;

public:
    Ingredient(std::string name, int quantity);
    ~Ingredient();

    int getName();
    int getQuantity();

    void setName(int name);
    void setQuantity(int quantity);
};
