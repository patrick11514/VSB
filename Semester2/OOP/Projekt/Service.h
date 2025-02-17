#pragma once

#include "Employee.h"

#include <string>

/**
 * @brief The Service class
 */
class Service : public Employee
{
public:
    /**
     * @brief Constructor
     * @param name Name of the service
     * @param surname Surname of the service
     * @param age Age of the service
     */
    Service(std::string name, std::string surname, int age);
    /**
     * @brief Destructor
     */
    ~Service();
};
