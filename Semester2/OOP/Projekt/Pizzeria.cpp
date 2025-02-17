#include "Pizzeria.h"
#include "Employee.h"
#include "Pizza.h"
#include "Order.h"

#include <string>

Pizzeria::Pizzeria(std::string name, int maxEmployeesCount, int maxOrdersCount)
{
    this->name = name;
    this->maxEmployeesCount = maxEmployeesCount;
    this->maxOrdersCount = maxOrdersCount;
    this->employeesCount = 0;
    this->ordersCount = 0;
    this->employees = new Employee *[maxEmployeesCount];
    this->orders = new Order *[maxOrdersCount];
}

Pizzeria::~Pizzeria()
{
    for (int i = 0; i < this->employeesCount; i++)
    {
        delete this->employees[i];
    }
    delete[] this->employees;

    for (int i = 0; i < this->ordersCount; i++)
    {
        delete this->orders[i];
    }
    delete[] this->orders;
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

void Pizzeria::addOrder(Order *order)
{
    if (this->ordersCount < this->maxOrdersCount)
    {
        this->orders[this->ordersCount++] = order;
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

Order *Pizzeria::getOrder(int index)
{
    if (index >= 0 && index < this->ordersCount)
    {
        return this->orders[index];
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

void Pizzeria::removeOrder(int index)
{
    if (index >= 0 && index < this->ordersCount)
    {
        delete this->orders[index];
        for (int i = index; i < this->ordersCount - 1; i++)
        {
            this->orders[i] = this->orders[i + 1];
        }
        this->ordersCount--;
    }
}

std::string Pizzeria::printEmployees()
{
    std::string result = "";
    for (int i = 0; i < this->employeesCount; i++)
    {
        result += this->employees[i]->getName() + " " + this->employees[i]->getSurname() + ", Age: " + std::to_string(this->employees[i]->getAge()) + "\n";
    }
    return result;
}

std::string Pizzeria::printOrders()
{
    std::string result = "";
    for (int i = 0; i < this->ordersCount; i++)
    {
        result += this->orders[i]->getName() + " " + this->orders[i]->getAddress() + ":\n";
        for (int l = 0; l < this->orders[i]->getPizzasCount(); l++)
        {
            Pizza *pizza = this->orders[i]->getPizzas()[l];
            result += "- " + pizza->getName() + " " + std::to_string(pizza->getPrice()) + "Kč\n";
        }

        result += "Sum: " + std::to_string(this->orders[i]->getPrice()) + "Kč\n";
    }
    return result;
}