#pragma once

#include "Employee.h"
#include "Pizza.h"
#include "Order.h"

#include <string>

/**
 * @brief Class that represents a pizzeria
 */
class Pizzeria
{
private:
    Employee **employees;  ///< Array of employees
    int employeesCount;    ///< Number of employees
    int maxEmployeesCount; ///< Maximum number of employees
    Order **orders;        ///< Array of orders
    int ordersCount;       ///< Number of orders
    int maxOrdersCount;    ///< Maximum number of orders

    std::string name; ///< Name of pizzeria

public:
    /**
     * @brief Constructor
     * @param name Name of pizzeria
     * @param maxEmployeesCount Maximum number of employees
     * @param maxOrderCount Maximum number of orders
     */
    Pizzeria(std::string name, int maxEmployeesCount, int maxOrderCount);
    /**
     * @brief Destructor
     */
    ~Pizzeria();

    /**
     * @brief Get name of pizzeria
     * @return Name of pizzeria
     */
    std::string getName();

    /**
     * @brief Set name of pizzeria
     * @param name Name of pizzeria
     */
    void setName(std::string name);

    /**
     * @brief Add employee to pizzeria
     * @param employee Pointer of employee to add
     */
    void addEmployee(Employee *employee);
    /**
     * @brief Add order to pizzeria
     * @param order Pointer of order to add
     */
    void addOrder(Order *order);

    /**
     * @brief Get employee from pizzeria
     * @param index Index of employee to get
     * @return Pointer of employee
     */
    Employee *getEmployee(int index);
    /**
     * @brief Get order from pizzeria
     * @param index Index of order to get
     * @return Pointer of order
     */
    Order *getOrder(int index);

    /**
     * @brief Remove employee from pizzeria
     * @param index Index of employee to remove
     */
    void removeEmployee(int index);
    /**
     * @brief Remove order from pizzeria
     * @param index Index of order to remove
     */
    void removeOrder(int index);

    /**
     * @brief Get employees as string
     * @return Employees as string
     */
    std::string printEmployees();
    /**
     * @brief Get orders as string
     * @return Orders as string
     */
    std::string printOrders();
};
