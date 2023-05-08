#pragma once

#include "Employee.h"

#include <string>

class Chef : public Employee
{
public:
    Chef(std::string name, std::string surname, int age);
    ~Chef();
};
