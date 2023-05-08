#include "Chef.h"
#include "Order.h"

Chef::Chef(std::string name, std::string surname, int age) : Employee(name, surname, age)
{
    this->orderCount = 0;
    this->maxOrderCount = 0;

    this->order = nullptr;
}

Chef::Chef(std::string name, std::string surname, int age, int maxOrderCount) : Employee(name, surname, age)
{
    this->orderCount = 0;
    this->maxOrderCount = maxOrderCount;

    this->order = new Order *[maxOrderCount];
}

Chef::~Chef()
{
    delete[] this->order;
}

void Chef::assingOrder(Order *order)
{
    if (this->orderCount < this->maxOrderCount)
    {
        this->order[this->orderCount] = order;
        this->orderCount++;
    }
}

void Chef::removeOrder(int index)
{
    if (index >= 0 && index < this->orderCount)
    {
        for (int i = index; i < this->orderCount - 1; i++)
        {
            this->order[i] = this->order[i + 1];
        }

        this->orderCount--;
    }
}

void Chef::setMaxOrderCount(int maxOrderCount)
{
    if (this->orderCount <= maxOrderCount)
    {
        this->maxOrderCount = maxOrderCount;

        // resize array
        Order **temp = new Order *[maxOrderCount];

        for (int i = 0; i < this->orderCount; i++)
        {
            temp[i] = this->order[i];
        }

        delete[] this->order;
        this->order = temp;
    }
    else
    {
        // resize array
        Order **temp = new Order *[maxOrderCount];

        for (int i = 0; i < maxOrderCount; i++)
        {
            temp[i] = this->order[i];
        }

        delete[] this->order;
        this->order = temp;

        this->orderCount = maxOrderCount;
        this->maxOrderCount = maxOrderCount;
    }
}

int Chef::getMaxOrderCount()
{
    return this->maxOrderCount;
}

Order *Chef::getOrder(int index)
{
    if (index >= 0 && index < this->orderCount)
    {
        return this->order[index];
    }

    return nullptr;
}