#pragma once

#include "Ingredient.h"

/**
 * @brief The Vegetables class
 */
class Vegetables : public Ingredient
{
public:
    /**
     * @brief Constructor
     * @param quantity Quantity of vegetables
     */
    Vegetables(int quantity);
    /**
     * @brief Destructor
     */
    ~Vegetables();
};
