#pragma once

#include "Pizza.h"

/**
 * @brief Custom Pizza class
 */
class Custom : public Pizza
{
public:
    /**
     * @brief Constructor
     * @param name Name of pizza
     * @param bonusIngredientsCount Number of bonus ingredients
     */
    Custom(std::string name, int bonusIngredientsCount);
    /**
     * @brief Destructor
     */
    ~Custom();

    /**
     * @brief Add cheese to pizza
     * @param quantity Quantity of cheese
     * @param type Type of cheese
     */
    void addCheese(int quantity, std::string type);
    /**
     * @brief Add meat to pizza
     * @param quantity Quantity of meat
     */
    void addMeat(int quantity);
    /**
     * @brief Add vegetables to pizza
     * @param quantity Quantity of vegetables
     */
    void addVegetables(int quantity);
};
