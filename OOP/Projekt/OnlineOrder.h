#pragma once

#include "Order.h"

/**
 * @brief The OnlineOrder class
 */
class OnlineOrder : public Order
{
private:
    int deliveryPrice; ///< Delivery price

public:
    /**
     * @brief Constructor
     * @param name Name of customer
     * @param address Address of customer
     * @param phone Phone of customer
     * @param maxPizzasCount Maximum number of pizzas
     * @param deliveryPrice Delivery price
     */
    OnlineOrder(std::string name, std::string address, std::string phone, int maxPizzasCount, int deliveryPrice);
    /**
     * @brief Destructor
     */
    ~OnlineOrder();

    /**
     * @brief Get delivery price
     * @return Delivery price
     */
    virtual int getPrice();

    /**
     * @brief Set delivery price
     * @param deliveryPrice Delivery price
     */
    int getDeliveryPrice();
};
