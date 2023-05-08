#pragma once

#include "Employee.h"

#include <string>

class Service : public Employee
{
public:
    Service(std::string name, std::string surname, int age);
    ~Service();
};
