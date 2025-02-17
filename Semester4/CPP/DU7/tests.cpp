// This tells doctest to provide a main() function
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "tasks.h"

#include <memory>

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

TEST_CASE("Find best 3 students")
{
        std::vector<Student> students = get_students();
        Exam exam{"SKJ", 10, 80};

        auto best3 = find_best_n_students(students, exam, 3);

        decltype(best3) expected{
            {students[1], 208},
            {students[6], 162},
            {students[2], 144}};
        REQUIRE(best3 == expected);
}

TEST_CASE("Find best n larger than student count")
{
        std::vector<Student> students = get_students();
        Exam exam{"SKJ", 10, 80};

        auto best_n = find_best_n_students(students, exam, students.size() + 1);

        decltype(best_n) expected{
            {students[1], 208},
            {students[6], 162},
            {students[2], 144},
            {students[5], 132},
            {students[7], 110},
            {students[0], 102},
            {students[3], 72},
            {students[4], 50}};
        REQUIRE(best_n == expected);
}

TEST_CASE("Find best 0 students")
{
        std::vector<Student> students = get_students();
        Exam exam{"SKJ", 10, 80};

        auto best = find_best_n_students(students, exam, 0);
        REQUIRE(best.empty());
}

TEST_CASE("No students")
{
        std::vector<Student> students;
        std::vector<Exam> exams{
            Exam{"SKJ", 40, 50},
            Exam{"C++I", 20, 90}};

        REQUIRE(max_score_difference(students, exams) == 0);
}

TEST_CASE("No exams")
{
        std::vector<Student> students = get_students();
        std::vector<Exam> exams;

        REQUIRE(max_score_difference(students, exams) == 0);
}

TEST_CASE("No exams and no students")
{
        std::vector<Student> students;
        std::vector<Exam> exams;

        REQUIRE(max_score_difference(students, exams) == 0);
}

TEST_CASE("Single exam")
{
        std::vector<Student> students = get_students();
        std::vector<Exam> exams{
            Exam{"C++I", 20, 90}};

        REQUIRE(max_score_difference(students, exams) == 44);
}

TEST_CASE("Multiple exams 1")
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

        REQUIRE(max_score_difference(students, exams) == 109);
}

TEST_CASE("Multiple exams 2")
{
        std::vector<Student> students{
            Student{"A", 91, 5},
            Student{"B", 92, 5},
            Student{"C", 93, 5},
        };
        std::vector<Exam> exams{
            Exam{"A", 80, 35},
            Exam{"B", 81, 35},
            Exam{"C", 82, 35},
        };

        REQUIRE(max_score_difference(students, exams) == 1);
}

TEST_CASE("Multiple exams 3")
{
        std::vector<Student> students{
            Student{"A", 111, 5},
            Student{"B", 110, 5},
            Student{"C", 90, 5},
            Student{"D", 70, 5},
            Student{"E", 50, 5},
        };
        std::vector<Exam> exams{
            Exam{"A", 80, 35},
            Exam{"B", 60, 35},
            Exam{"C", 40, 35},
        };

        REQUIRE(max_score_difference(students, exams) == 24);
}

static std::unordered_set<Student> to_set(std::vector<Student> students)
{
        std::unordered_set<Student> set;
        std::copy(students.begin(), students.end(), std::inserter(set, set.begin()));
        return set;
}

TEST_CASE("Filter students no exams")
{
        std::vector<Student> students{
            Student{"Jana", 10, 50},
            Student{"Martin", 30, 70}};

        auto [all, one] = filter_students(students, {});
        REQUIRE(all == to_set(students));
        REQUIRE(one.empty());
}

TEST_CASE("Filter students none passed")
{
        std::vector<Student> students{
            Student{"Jana", 10, 10},
            Student{"Martin", 10, 20},
            Student{"Marek", 10, 20}};
        std::vector<Exam> exams{
            Exam{"SKJ", 90, 90},
            Exam{"APPS", 80, 90},
            Exam{"APPS", 70, 90}};

        auto [all, one] = filter_students(students, exams);
        REQUIRE(all.empty());
        REQUIRE(one.empty());
}

TEST_CASE("Filter students all passed")
{
        std::vector<Student> students{
            Student{"Jana", 90, 80},
            Student{"Martin", 80, 90},
            Student{"Marek", 80, 85}};
        std::vector<Exam> exams{
            Exam{"SKJ", 20, 20},
            Exam{"APPS", 30, 40},
            Exam{"UPR", 10, 50}};

        auto [all, one] = filter_students(students, exams);
        REQUIRE(all == to_set(students));
        REQUIRE(one == to_set(students));
}

TEST_CASE("Filter students some passed")
{
        std::vector<Student> students{
            Student{"Jana", 100, 100},
            Student{"Martin", 10, 20},
            Student{"Marek", 30, 40}};
        std::vector<Exam> exams{
            Exam{"SKJ", 30, 90},
            Exam{"MA", 90, 10},
            Exam{"APPS", 20, 20}};

        auto [all, one] = filter_students(students, exams);
        REQUIRE(all == to_set({students[0]}));
        REQUIRE(one == to_set({students[0], students[2]}));
}

TEST_CASE("Leaderboard single exam per subject")
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
        auto leaderboard = get_leaderboard_of_each_subject(students, exams);
        REQUIRE(leaderboard.size() == 4);
        REQUIRE(leaderboard["SKJ"] == std::vector<std::pair<Student, Score>>{
                                          {students[1], 174},
                                          {students[0], 106},
                                          {students[2], 72}});
        REQUIRE(leaderboard["DIM"] == std::vector<std::pair<Student, Score>>{
                                          {students[1], 72},
                                          {students[2], 66},
                                          {students[0], 54}});
        REQUIRE(leaderboard["MA"] == std::vector<std::pair<Student, Score>>{
                                         {students[2], 148},
                                         {students[0], 74},
                                         {students[1], 66}});
        REQUIRE(leaderboard["APPS"] == std::vector<std::pair<Student, Score>>{
                                           {students[2], 212},
                                           {students[1], 204},
                                           {students[0], 160}});
}

TEST_CASE("Leaderboard multiple exams per subject")
{
        std::vector<Student> students{
            Student{"Jarmila", 50, 40},
            Student{"Lenka", 90, 30},
            Student{"Jakub", 20, 90},
        };
        std::vector<Exam> exams{
            Exam{"SKJ", 10, 80},
            Exam{"APPS", 10, 90},
            Exam{"DIM", 70, 70},
            Exam{"MA", 90, 20},
            Exam{"APPS", 20, 100},
            Exam{"MA", 100, 10},
            Exam{"DIM", 40, 60},
            Exam{"MA", 100, 0},
            Exam{"APPS", 10, 100},
            Exam{"SKJ", 20, 90},
            Exam{"SKJ", 20, 90},
        };
        auto leaderboard = get_leaderboard_of_each_subject(students, exams);
        REQUIRE(leaderboard.size() == 4);
        REQUIRE(leaderboard["SKJ"] == std::vector<std::pair<Student, Score>>{
                                          {students[1], 474},
                                          {students[0], 282},
                                          {students[2], 172}});
        REQUIRE(leaderboard["DIM"] == std::vector<std::pair<Student, Score>>{
                                          {students[1], 204},
                                          {students[2], 162},
                                          {students[0], 146}});
        REQUIRE(leaderboard["MA"] == std::vector<std::pair<Student, Score>>{
                                         {students[2], 490},
                                         {students[0], 228},
                                         {students[1], 182}});
        REQUIRE(leaderboard["APPS"] == std::vector<std::pair<Student, Score>>{
                                           {students[1], 474},
                                           {students[0], 268},
                                           {students[2], 124}});
}

TEST_CASE("Leaderboard order changes")
{
        std::vector<Student> students{
            Student{"Jarmila", 50, 10},
            Student{"Lenka", 20, 75},
        };
        std::vector<Exam> exams{
            Exam{"DIM", 10, 65},
            Exam{"DIM", 85, 5},
            Exam{"DIM", 90, 10},
            Exam{"DIM", 90, 10},
        };
        auto leaderboard = get_leaderboard_of_each_subject(students, exams);
        REQUIRE(leaderboard.size() == 1);
        REQUIRE(leaderboard["DIM"] == std::vector<std::pair<Student, Score>>{
                                          {students[1], 514},
                                          {students[0], 187}});
}