#include "Pizzeria.h"
#include "Employee.h"
#include "Pizza.h"

#include <string>

Pizzeria::Pizzeria(std::string name, int maxEmployeesCount, int maxPizzasCount)
{
    this->name = name;
    this->maxEmployeesCount = maxEmployeesCount;
    this->maxPizzasCount = maxPizzasCount;
    this->employeesCount = 0;
    this->pizzasCount = 0;
    this->employees = new Employee *[maxEmployeesCount];
    this->pizzas = new Pizza *[maxPizzasCount];
}

Pizzeria::~Pizzeria()
{
    for (int i = 0; i < this->employeesCount; i++)
    {
        delete this->employees[i];
    }
    delete[] this->employees;

    for (int i = 0; i < this->pizzasCount; i++)
    {
        delete this->pizzas[i];
    }
    delete[] this->pizzas;
}

std::string Pizzeria::getName()
{
    return this->name;
}

void Pizzeria::setName(std::string name)
{
    this->name = name;
}

void Pizzeria::addEmployee(Employee *employee)
{
    if (this->employeesCount < this->maxEmployeesCount)
    {
        this->employees[this->employeesCount++] = employee;
    }
}

void Pizzeria::addPizza(Pizza *pizza)
{
    if (this->pizzasCount < this->maxPizzasCount)
    {
        this->pizzas[this->pizzasCount++] = pizza;
    }
}

Employee *Pizzeria::getEmployee(int index)
{
    if (index >= 0 && index < this->employeesCount)
    {
        return this->employees[index];
    }
    return nullptr;
}

Pizza *Pizzeria::getPizza(int index)
{
    if (index >= 0 && index < this->pizzasCount)
    {
        return this->pizzas[index];
    }
    return nullptr;
}

void Pizzeria::removeEmployee(int index)
{
    if (index >= 0 && index < this->employeesCount)
    {
        delete this->employees[index];
        for (int i = index; i < this->employeesCount - 1; i++)
        {
            this->employees[i] = this->employees[i + 1];
        }
        this->employeesCount--;
    }
}

void Pizzeria::removePizza(int index)
{
    if (index >= 0 && index < this->pizzasCount)
    {
        delete this->pizzas[index];
        for (int i = index; i < this->pizzasCount - 1; i++)
        {
            this->pizzas[i] = this->pizzas[i + 1];
        }
        this->pizzasCount--;
    }
}

std::string Pizzeria::printEmployees()
{
    std::string result = "";
    for (int i = 0; i < this->employeesCount; i++)
    {
        result += this->employees[i]->getName() + " " + this->employees[i]->getSurname() + " " + std::to_string(this->employees[i]->getAge()) + "\n";
    }
    return result;
}

std::string Pizzeria::printPizzas()
{
    std::string result = "";
    for (int i = 0; i < this->pizzasCount; i++)
    {
        result += this->pizzas[i]->getName() + " " + std::to_string(this->pizzas[i]->getPrice()) + "\n";
    }
    return result;
}