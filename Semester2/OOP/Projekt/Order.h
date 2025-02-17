#pragma once

#include "AbstractOrder.h"
#include "Pizza.h"

#include <string>

/**
 * @brief Order class
 */
class Order : public AbstractOrder
{
private:
    Pizza **pizzas;     ///< Array of pizzas
    int pizzasCount;    ///< Count of pizzas
    int maxPizzasCount; ///< Maximum number of pizzas

    std::string name;    ///< Name of customer
    std::string address; ///< Address of customer
    std::string phone;   ///< Phone of customer

public:
    /**
     * @brief Constructor
     * @param name Name of customer
     * @param address Address of customer
     * @param phone Phone of customer
     * @param maxPizzasCount Maximum number of pizzas
     */
    Order(std::string name, std::string address, std::string phone, int maxPizzasCount);
    /**
     * @brief Destructor
     */
    ~Order();

    /**
     * @brief Get price
     * @return Price
     */
    virtual int getPrice();

    /**
     * @brief Add pizza
     * @param pizza Pointer of pizza to add
     */
    void addPizza(Pizza *pizza);
    /**
     * @brief Remove pizza
     * @param index Index of pizza to remove
     */
    void removePizza(int index);

    /**
     * @brief Get pizzas
     * @return Array of pizzas
     */
    Pizza **getPizzas();

    /**
     * @brief Get name
     * @return Name
     */
    std::string getName();
    /**
     * @brief Get address
     * @return Address
     */
    std::string getAddress();
    /**
     * @brief Get phone
     * @return Phone
     */
    std::string getPhone();

    /**
     * @brief Get pizzas count
     * @return Pizzas count
     */
    int getPizzasCount();
};
