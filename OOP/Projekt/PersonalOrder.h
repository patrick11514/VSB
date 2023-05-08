#pragma once

#include "Order.h"

/**
 * @brief The PersonalOrder class
 */
class PersonalOrder : public Order
{
public:
    /**
     * @brief Constructor
     * @param name Name of the customer
     * @param address Address of the customer
     * @param phone Phone of the customer
     * @param maxPizzasCount Maximum number of pizzas
     */
    PersonalOrder(std::string name, std::string address, std::string phone, int maxPizzasCount);
    /**
     * @brief Destructor
     */
    ~PersonalOrder();
};
