#include "OnlineOrder.h"
#include "Order.h"
#include "Pizza.h"

OnlineOrder::OnlineOrder(std::string name, std::string address, std::string phone, int maxPizzasCount, int deliveryPrice) : Order(name, address, phone, maxPizzasCount)
{
    this->deliveryPrice = deliveryPrice;
}

OnlineOrder::~OnlineOrder()
{
}

int OnlineOrder::getPrice()
{
    Pizza **pizzas = this->getPizzas();

    int price = 0;

    for (int i = 0; i < this->getPizzasCount(); i++)
    {
        price += pizzas[i]->getPrice();
    }

    return price + this->deliveryPrice;
}

int OnlineOrder::getDeliveryPrice()
{
    return this->deliveryPrice;
}