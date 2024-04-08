#pragma once

#include <algorithm>
#include <cstring>
#include <fstream>
#include <numeric>
#include <ostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iostream>

struct Student
{
    explicit Student(
        std::string_view name,
        size_t math_skill = 1,
        size_t coding_skill = 1
    ) : name(name),
        math_skill(math_skill),
        coding_skill(coding_skill)
    {}

    bool operator<(const Student& other) const
    {
        return this->name < other.name;
    }
    bool operator==(const Student& other) const
    {
        return this->name == other.name;
    }

    std::string_view name;

    size_t math_skill;
    size_t coding_skill;
};

std::ostream& operator<<(std::ostream& os, const Student& student);

using Subject = std::string;

struct Exam
{
    Exam(
        Subject subject,
        size_t math_difficulty,
        size_t coding_difficulty
    ) : subject(std::move(subject)),
        math_difficulty(math_difficulty),
        coding_difficulty(coding_difficulty)
    {}

    bool operator==(const Exam& other) const {
        return (
            this->subject == other.subject &&
            this->math_difficulty == other.math_difficulty &&
            this->coding_difficulty == other.coding_difficulty
        );
    }

    Subject subject;
    size_t math_difficulty;
    size_t coding_difficulty;
};

// Hashing support for Student
namespace std {
    template <>
    struct hash<Student>
    {
        std::size_t operator()(const Student& student) const
        {
            return std::hash<std::string_view>{}(student.name);
        }
    };
}

std::ostream& operator<<(std::ostream& os, const Exam& exam);

using Score = int;

/**
 * Calculates the score of a student on a specific exam.
 * Note: implemented in tasks.cpp.
 */
Score calculate_score(Student student, Exam exam);

// TODO
std::vector<std::pair<Student, Score>> find_best_n_students(
    std::vector<Student> students,
    const Exam& exam,
    size_t n
);

// TODO
size_t max_score_difference(
    const std::vector<Student>& students,
    const std::vector<Exam>& exams
);

// TODO
std::pair<std::unordered_set<Student>, std::unordered_set<Student>> filter_students(
    const std::vector<Student>& students,
    const std::vector<Exam>& exams
);

using Leaderboard = std::unordered_map<Subject, std::vector<std::pair<Student, Score>>>;

// TODO
Leaderboard get_leaderboard_of_each_subject(
    const std::vector<Student>& students,
    const std::vector<Exam>& exams
);
