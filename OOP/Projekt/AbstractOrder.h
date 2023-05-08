#pragma once

/**
 * @brief The AbstractOrder class
 */

class AbstractOrder
{
public:
    AbstractOrder();
    virtual ~AbstractOrder() = 0;

    virtual int getPrice() = 0;
};
