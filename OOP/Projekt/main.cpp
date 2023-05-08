#include <iostream>
#include <string>

#include "Pizzeria.h"
#include "OnlineOrder.h"
#include "PersonalOrder.h"
#include "Vegetarian.h"
#include "Custom.h"
#include "Service.h"
#include "Chef.h"
#include "Pizza.h"

/**
 * Pizzeria OOP Project
 * @author Patrik Mintěl (MIN0150)
 * @version 1.2
 * @date 8.5.2023
 */

int main()
{
    // ==================================================
    // create pizzeria
    std::cout << "===========================================" << std::endl;
    std::cout << "Creating pizzeria with name \"Patrikova Pizzerie\" and max employees count 2 and max orders count 3" << std::endl;
    Pizzeria pizzeria("Patrikova Pizzerie", 2, 3);

    // ==================================================

    // create first personal order
    std::cout << "Creating personal order with name \"Viktor Strakoň\", address \"Sámova 156/7, Frýdek-Místek\", phone \"123456789\" and max pizzas count 2" << std::endl;
    PersonalOrder *personalOrder = new PersonalOrder("Viktor Strakoň", "Sámova 156/7, Frýdek-Místek", "123456789", 2);

    // add vegetarian pizza
    std::cout << "Adding vegetarian pizza" << std::endl;
    personalOrder->addPizza(new Vegetarian());

    // add custom pizza
    std::cout << "Adding custom pizza with 1 mozarella cheese, 2 chedar cheeses and 1 meat" << std::endl;
    Custom *customPizza = new Custom("Custom", 3);
    // add mozarella cheese
    customPizza->addCheese(1, "Mozzarella");
    // add chedar cheese
    customPizza->addCheese(2, "Chedar");
    // add meat
    customPizza->addMeat(1);
    // add pizza to order
    personalOrder->addPizza(customPizza);

    // add order to pizzeria
    pizzeria.addOrder(personalOrder);

    // ==================================================

    // Create second online order
    std::cout << "Creating online order with name \"Tonda Toník\", address \"Anny Morávkové 456, Havířov\", phone \"123456789\", max pizzas count 3 and delivery price 99" << std::endl;
    OnlineOrder *onlineOrder = new OnlineOrder("Tonda Toník", "Anny Morávkové 456, Havířov", "123456789", 3, 99);

    // add custom pizza
    std::cout << "Adding custom pizza with 2 meats and 3 vegetables" << std::endl;
    Custom *customPizza2 = new Custom("Custom", 2);
    // add meat
    customPizza2->addMeat(2);
    // add vegetables
    customPizza2->addVegetables(3);

    // add pizza to order
    onlineOrder->addPizza(customPizza2);

    // add two vegetarian pizzas
    std::cout << "Adding two vegetarian pizzas" << std::endl;
    onlineOrder->addPizza(new Vegetarian());
    onlineOrder->addPizza(new Vegetarian());

    // remove one vegetarian pizza
    std::cout << "Removing one vegetarian pizza" << std::endl;
    onlineOrder->removePizza(1);

    // add custom order instead
    std::cout << "Adding custom pizza with 3 vegetables and 2 carski cheeses" << std::endl;
    Custom *customPizza3 = new Custom("Custom", 2);
    // add vegetables
    customPizza3->addVegetables(3);
    // add cheese
    customPizza3->addCheese(2, "Carski");

    // add pizza to order
    onlineOrder->addPizza(customPizza3);

    // add order to pizzeria
    pizzeria.addOrder(onlineOrder);

    // ==================================================
    std::cout << "===========================================" << std::endl;

    // add employee

    // create employee
    std::cout << "Creating employee (Chef) with name \"Patrik Pawlovski\" and age 20 and with 1 max order count" << std::endl;
    Chef *chef = new Chef("Patrik", "Pawlovski", 20);
    // set order count
    chef->setMaxOrderCount(1);
    // assign order to employee
    std::cout << "Assigning first order to employee" << std::endl;
    chef->assingOrder(pizzeria.getOrder(0));

    // add employee to pizzeria
    pizzeria.addEmployee(chef);

    // add employee
    std::cout << "Creating employee (Service) with name \"Tonda Honza\" and age 22" << std::endl;
    Service *service = new Service("Tonda", "Honza", 22);

    // add employee to pizzeria
    pizzeria.addEmployee(service);

    // ==================================================
    std::cout << "===========================================" << std::endl;

    // print pizzeria
    // print orders
    std::cout << "Orders in pizzeria:" << std::endl;
    std::cout << pizzeria.printOrders() << std::endl;

    // print employees
    std::cout << "Employees in pizzeria:" << std::endl;
    std::cout << pizzeria.printEmployees() << std::endl;

    // ==================================================

    // Print second pizza in first order
    std::cout << "Second pizza in first order:" << std::endl;
    Pizza *pizza = pizzeria.getOrder(0)->getPizzas()[1];
    std::cout << "Name: " << pizza->getName() << std::endl;
    std::cout << "Ingredients:" << std::endl;
    for (int i = 0; i < pizza->getIngredientsCount(); i++)
    {
        Ingredient *ing = pizza->getIngredient(i);
        std::cout << "- " << ing->getQuantity() << "x " << ing->getName() << std::endl;
    }

    std::cout << std::endl;

    // ==================================================

    // print order of chef
    std::cout << "Order of chef:" << std::endl;
    Order *chefOrder = chef->getOrder(0);
    std::cout << "Name: " << chefOrder->getName() << std::endl;
    std::cout << "Address: " << chefOrder->getAddress() << std::endl;
    std::cout << "Phone: " << chefOrder->getPhone() << std::endl;
    std::cout << "Pizzas:" << std::endl;
    for (int i = 0; i < chefOrder->getPizzasCount(); i++)
    {
        Pizza *pizza = chefOrder->getPizzas()[i];
        std::cout << "- " << pizza->getName() << std::endl;
    }

    std::cout << std::endl;

    // ==================================================

    // get total pizzas count
    std::cout << "Total pizzas count: " << Pizza::getClassCount() << std::endl;

    // ==================================================
    std::cout << std::endl;

    // delete second order
    std::cout << "Deleting second order" << std::endl;
    pizzeria.removeOrder(1);

    // ==================================================

    // print orders

    std::cout << "Orders in pizzeria:" << std::endl;
    std::cout << pizzeria.printOrders() << std::endl;

    // ==================================================

    // get total pizzas count
    std::cout << "Total pizzas count: " << Pizza::getClassCount() << std::endl;

    return 0;
}