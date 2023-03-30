#include <iostream>
#include <string>

#include "Student.h"
#include "Class.h"
#include "Subject.h"
#include "Teacher.h"
#include "School.h"
#include "ClassRoom.h"

int main()
{
    School *school = new School("School");
    Class *c1 = school->createClass("1.A");
    Class *c2 = school->createClass("2.B");

    school->createTeacher("Jan", "Novak");
    school->createTeacher("Petr", "Novak");

    Subject *s1 = school->createSubject("Matematika", "9:00", "9:45", school->getTeachers()[0]);
    Subject *s2 = school->createSubject("Angličtina", "10:00", "10:45", school->getTeachers()[0]);
    Subject *s3 = school->createSubject("Český jazyk", "11:00", "11:45", school->getTeachers()[1]);
    Subject *s4 = school->createSubject("Fyzika", "12:00", "12:45", school->getTeachers()[1]);

    school->createClassRoom("B110", school->getClasses()[0])->addSubject(s1)->addSubject(s3);
    school->createClassRoom("B120", school->getClasses()[1])->addSubject(s2)->addSubject(s4);

    Student *st1 = school->createStudent("Patrik", "Mintěl");
    Student *st2 = school->createStudent("Pepa", "Novák");
    Student *st3 = school->createStudent("Tom", "Kašpárek");
    Student *st4 = school->createStudent("Petr", "Kratochvíl");

    c1->addStudent(st1)->addStudent(st2);

    delete school;

    return 0;
}