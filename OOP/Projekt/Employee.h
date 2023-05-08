#pragma once

#include "AbstractEmployee.h"

#include <string>

/**
 * @brief The Employee class
 */
class Employee : public AbstractEmployee
{
private:
    std::string name;    ///< Name of employee
    std::string surname; ///< Surname of employee
    int age;             ///< Age of employee

public:
    /**
     * @brief Constructor
     * @param name Name of employee
     * @param surname Surname of employee
     * @param age Age of employee
     */
    Employee(std::string name, std::string surname, int age);
    /**
     * @brief Destructor
     */
    ~Employee();

    /**
     * @brief Get name of employee
     * @return Name of employee
     */
    std::string getName();
    /**
     * @brief Get surname of employee
     * @return Surname of employee
     */
    std::string getSurname();
    /**
     * @brief Get age of employee
     * @return Age of employee
     */
    int getAge();

    /**
     * @brief Set name of employee
     * @param name Name of employee
     */
    void setName(std::string name);
    /**
     * @brief Set surname of employee
     * @param surname Surname of employee
     */
    void setSurname(std::string surname);
    /**
     * @brief Set age of employee
     * @param age Age of employee
     */
    void setAge(int age);
};
