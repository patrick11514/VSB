#ifndef __Subject_H_
#define __Subject_H_ 1

#include <string>
#include "Teacher.h"

using namespace std;

class Subject
{
private:
    string name;
    string from;
    string to;
    Teacher *teacher;

public:
    Subject(string name, string from, string to, Teacher *teacher);

    string getName();
    string getTime();
    Teacher *getTeacher();

    void setFrom(string from);
    void setTo(string to);
};

#endif
