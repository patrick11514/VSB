#include "Order.h"
#include "Pizza.h"

#include <string>

Order::Order(std::string name, std::string address, std::string phone, int maxPizzasCount)
{
    this->name = name;
    this->address = address;
    this->phone = phone;
    this->maxPizzasCount = maxPizzasCount;
    this->pizzas = new Pizza *[maxPizzasCount];
    this->pizzasCount = 0;
}

Order::~Order()
{
    for (int i = 0; i < this->pizzasCount; i++)
    {
        delete this->pizzas[i];
    }
    delete[] this->pizzas;
}

int Order::getPrice()
{
    int price = 0;

    for (int i = 0; i < this->pizzasCount; i++)
    {
        price += this->pizzas[i]->getPrice();
    }

    return price;
}

void Order::addPizza(Pizza *pizza)
{
    if (this->pizzasCount < this->maxPizzasCount)
    {
        this->pizzas[this->pizzasCount++] = pizza;
    }
}

void Order::removePizza(int index)
{
    if (index < 0 || index >= this->pizzasCount)
    {
        return;
    }

    delete this->pizzas[index];

    for (int i = index; i < this->pizzasCount - 1; i++)
    {
        this->pizzas[i] = this->pizzas[i + 1];
    }

    this->pizzasCount--;
}

std::string Order::getName()
{
    return this->name;
}

std::string Order::getAddress()
{
    return this->address;
}

std::string Order::getPhone()
{
    return this->phone;
}

Pizza **Order::getPizzas()
{
    return this->pizzas;
}

int Order::getPizzasCount()
{
    return this->pizzasCount;
}