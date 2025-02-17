#pragma once

#include "Employee.h"
#include "Order.h"

#include <string>

/**
 * @brief The Chef class
 */
class Chef : public Employee
{
private:
    Order **order;     ///< Array of orders
    int orderCount;    ///< Number of orders
    int maxOrderCount; ///< Maximum number of orders

public:
    /**
     * @brief Constructor
     * @param name Name of chef
     * @param surname Surname of chef
     * @param age Age of chef
     */
    Chef(std::string name, std::string surname, int age);
    /**
     * @brief Constructor
     * @param name Name of chef
     * @param surname Surname of chef
     * @param age Age of chef
     * @param maxOrderCount Maximum number of orders
     */
    Chef(std::string name, std::string surname, int age, int maxOrderCount);
    /**
     * @brief Destructor
     */
    ~Chef();

    /**
     * @brief Assing order to chef
     * @param order Pointer of order to assing
     */
    void assingOrder(Order *order);
    /**
     * @brief Remove order from chef
     * @param index Index of order to remove
     */
    void removeOrder(int index);

    /**
     * @brief Set maximum number of orders
     * @param maxOrderCount Maximum number of orders
     */
    void setMaxOrderCount(int maxOrderCount);
    /**
     * @brief Get maximum number of orders
     * @return Maximum number of orders
     */
    int getMaxOrderCount();
    /**
     * @brief Get specific order
     * @param index Index of order
     * @return Pointer of order
     */
    Order *getOrder(int index);
};
