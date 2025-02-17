#ifndef __Student_H_
#define __Student_H_ 1

#include <string>

using namespace std;

class Student
{
private:
    string name;
    string surname;

public:
    Student(string name, string surname);

    string getName();
    string getSurname();
};

#endif
