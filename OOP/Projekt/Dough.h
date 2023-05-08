#pragma once

#include "Ingredient.h"

/**
 * @brief The Dough class
 */
class Dough : public Ingredient
{
public:
    /**
     * @brief Constructor
     * @param quantity Quantity of dough
     */
    Dough(int quantity);
    /**
     * @brief Destructor
     */
    ~Dough();
};
