#pragma once

#include "Employee.h"
#include "Pizza.h"

#include <string>

class Pizzeria
{
private:
    Employee **employees;
    int employeesCount;
    int maxEmployeesCount;
    Pizza **pizzas;
    int pizzasCount;
    int maxPizzasCount;

    std::string name;

public:
    Pizzeria(std::string name, int maxEmployeesCount, int maxPizzasCount);
    ~Pizzeria();

    std::string getName();

    void setName(std::string name);

    void addEmployee(Employee *employee);
    void addPizza(Pizza *pizza);

    Employee *getEmployee(int index);
    Pizza *getPizza(int index);

    void removeEmployee(int index);
    void removePizza(int index);

    std::string printEmployees();
    std::string printPizzas();
};
