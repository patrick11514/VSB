#include <iostream>
#include <string>
#include "HashTable.h"

int main()
{
    HashTable tabulka;

    tabulka["skola"] = 79;
    tabulka.insert("pepa", 8);
    tabulka.insert("karla", 42);

    std::cout << "skola: " << tabulka["skola"] << std::endl;
    std::cout << "Pepa: " << tabulka["pepa"] << std::endl;
    std::cout << "Karla: " << tabulka["karla"] << std::endl;

    tabulka.remove("pepa");

    std::cout << "Pepa: " << tabulka["pepa"] << std::endl;
    std::cout << "Karla: " << tabulka["karla"] << std::endl;

    tabulka.insert("pepa", 33);

    if (tabulka.contains("karla2"))
    {
        std::cout << "karla2 je v hashtable" << std::endl;
    }
    else
    {
        std::cout << "karla2 není v hashtable" << std::endl;
    }

    int vys = 0;

    if (tabulka.tryGetValue("pepa", &vys))
    {
        std::cout << "Pepa má: " << vys << std::endl;
    }

    tabulka.clear();

    std::cout << "Pepa: " << tabulka["pepa"] << std::endl;
    std::cout << "Karla: " << tabulka["karla"] << std::endl;

    return 0;
}
