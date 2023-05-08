#pragma once

#include <string>

class Employee
{
private:
    std::string name;
    std::string surname;
    int age;

public:
    Employee(std::string name, std::string surname, int age);
    ~Employee();

    std::string getName();
    std::string getSurname();
    int getAge();

    void setName(std::string name);
    void setSurname(std::string surname);
    void setAge(int age);
};
