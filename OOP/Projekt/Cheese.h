#pragma once

#include "Ingredient.h"

#include <string>

/**
 * @brief The Cheese class
 */
class Cheese : public Ingredient
{
private:
    std::string type; ///< Type of cheese

public:
    /**
     * @brief Constructor
     * @param quantity Quantity of cheese
     * @param type Type of cheese
     */
    Cheese(int quantity, std::string type);
    /**
     * @brief Destructor
     */
    ~Cheese();

    /**
     * @brief get type of cheese
     * @return type of cheese
     */
    std::string getType();

    /**
     * @brief set type of cheese
     * @param type type of cheese
     */
    void setType(std::string type);
};
