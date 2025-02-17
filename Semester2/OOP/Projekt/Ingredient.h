#pragma once

#include <string>

/**
 * @brief The Ingredient class
 */
class Ingredient
{
private:
    std::string name; ///< Name of ingredient
    int quantity;     ///< Quantity of ingredient

public:
    /**
     * @brief Constructor
     * @param name Name of ingredient
     * @param quantity Quantity of ingredient
     */
    Ingredient(std::string name, int quantity);
    /**
     * @brief Destructor
     */
    virtual ~Ingredient();

    /**
     * @brief Get name of ingredient
     * @return Name of ingredient
     */
    std::string getName();
    /**
     * @brief Get quantity of ingredient
     * @return Quantity of ingredient
     */
    int getQuantity();

    /**
     * @brief Set name of ingredient
     * @param name Name of ingredient
     */
    void setName(std::string name);
    /**
     * @brief Set quantity of ingredient
     * @param quantity Quantity of ingredient
     */
    void setQuantity(int quantity);
};
