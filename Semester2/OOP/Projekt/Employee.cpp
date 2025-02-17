#include "Employee.h"

Employee::Employee(std::string name, std::string surname, int age)
{
    this->name = name;
    this->surname = surname;
    this->age = age;
}

Employee::~Employee()
{
}

std::string Employee::getName()
{
    return this->name;
}

std::string Employee::getSurname()
{
    return this->surname;
}

int Employee::getAge()
{
    return this->age;
}

void Employee::setName(std::string name)
{
    this->name = name;
}

void Employee::setSurname(std::string surname)
{
    this->surname = surname;
}

void Employee::setAge(int age)
{
    this->age = age;
}
