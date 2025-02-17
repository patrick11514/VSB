#pragma once

#include "Ingredient.h"

/**
 * @brief The Sauce class
 */
class Sauce : public Ingredient
{
public:
    /**
     * @brief Constructor
     * @param quantity Quantity of sauce
     */
    Sauce(int quantity);
    /**
     * @brief Destructor
     */
    ~Sauce();
};
