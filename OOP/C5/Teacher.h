#ifndef __Teacher_H_
#define __Teacher_H_ 1

#include <string>
#include <vector>

using namespace std;

class Teacher
{
private:
    string name;
    string surname;

public:
    Teacher(string name, string surname);

    string getName();
    string getSurname();
};

#endif
