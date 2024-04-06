#include "tasks.h"

std::vector<Student> get_students()
{
    return {
        Student{"Jarmila", 50, 30},
        Student{"Lenka", 100, 70},
        Student{"Jakub", 60, 90},
        Student{"Martin", 20, 90},
        Student{"Michal", 10, 80},
        Student{"Jana", 60, 60},
        Student{"Petr", 70, 90},
        Student{"Konstantin", 50, 50},
    };
}

int main()
{

    std::vector<Student> students{
        Student{"Jarmila", 90, 5},
        Student{"Petr", 10, 90},
    };
    std::vector<Exam> exams{
        Exam{"DIM", 80, 35},
        Exam{"UPR", 5, 75},
        Exam{"ZPG", 65, 90},
    };

    std::cout << max_score_difference(students, exams) << std::endl;
}
