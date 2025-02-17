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
        Student{"Jarmila", 50, 40},
        Student{"Lenka", 90, 30},
        Student{"Jakub", 20, 90},
    };
    std::vector<Exam> exams{
        Exam{"SKJ", 10, 80},
        Exam{"DIM", 70, 70},
        Exam{"MA", 90, 20},
        Exam{"APPS", 20, 0}};

    auto result = get_leaderboard_of_each_subject(students, exams);

    for (const auto &pair : result)
    {
        std::cout << pair.first << std::endl;

        for (const auto &score : pair.second)
        {
            std::cout << score.first.name << ": " << score.second << std::endl;
        }
    }
}
