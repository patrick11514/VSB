#pragma once

#include <string>

#include "Ingredient.h"

/**
 * @brief The Pizza class
 */
class Pizza
{
private:
    std::string name;        ///< Name of pizza
    int maxIngredientsCount; ///< Maximum number of ingredients

    static int count; ///< Number of pizza classes

    int price; ///< Price of pizza

protected:
    Ingredient **ingredients; ///< Array of ingredients
    int ingredientsCount;     ///< Count of ingredients

public:
    /**
     * @brief Constructor
     * @param name Name of pizza
     * @param maxIngredientsCount Maximum number of ingredients
     */
    Pizza(std::string name, int maxIngredientsCount);
    /**
     * @brief Destructor
     */
    virtual ~Pizza();

    /**
     * @brief Get number of pizza classes
     * @return Number of pizza classes
     */
    static int getClassCount();

    /**
     * @brief Get name of pizza
     * @return Name of pizza
     */
    std::string getName();
    /**
     * @brief Get count of ingredients
     * @return Count of ingredients
     */
    int getIngredientsCount();
    /**
     * @brief Get pointer to ingredient
     * @param index Index of ingredient
     * @return Pointer to ingredient
     */
    Ingredient *getIngredient(int index);

    /**
     * @brief Add dough
     * @param quantity Quantity of dough
     */
    void addDough(int quantity);
    /**
     * @brief Add sauce
     * @param quantity Quantity of sauce
     */
    void addSauce(int quantity);

    /**
     * @brief Remove ingredient on index
     * @param index Index of ingredient
     */
    void removeIngredient(int index);

    /**
     * @brief Add default ingredients
     */
    virtual void addDefaultIngredients();

    /**
     * @brief Set price
     * @param price Price
     */
    void setPrice(int price);
    /**
     * @brief Get price
     * @return Price
     */
    int getPrice();
};
