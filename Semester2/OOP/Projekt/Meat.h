#pragma once

#include "Ingredient.h"

/**
 * @brief The Meat class
 */
class Meat : public Ingredient
{
public:
    /**
     * @brief Constructor
     * @param quantity Quantity of meat
     */
    Meat(int quantity);
    /**
     * @brief Destructor
     */
    ~Meat();
};
