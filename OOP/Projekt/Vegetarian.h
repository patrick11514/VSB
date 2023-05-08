#pragma once

#include "Pizza.h"

/**
 * @brief The Vegetarian pizza class
 */
class Vegetarian : public Pizza
{
public:
    /**
     * @brief Constructor
     */
    Vegetarian();
    /**
     * @brief Destructor
     */
    ~Vegetarian();

    /**
     * @brief Add default ingredients
     */
    virtual void addDefaultIngredients();

    /**
     * @brief Add cheese to pizza
     * @param quantity Quantity of cheese
     */
    void addVegetables(int quantity);
};
