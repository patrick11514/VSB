#include "tasks.h"
#include <string_view>

std::ostream &operator<<(std::ostream &os, const Student &student)
{
    os << student.name;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Exam &exam)
{
    os << exam.subject;
    return os;
}

Score calculate_score(Student student, Exam exam)
{
    int math_diff = static_cast<int>(student.math_skill) * std::max(1, 100 - static_cast<int>(exam.math_difficulty));
    int coding_diff = static_cast<int>(student.coding_skill) * std::max(1, 100 - static_cast<int>(exam.coding_difficulty));

    int diff = math_diff + coding_diff;
    return diff / 50;
}

std::vector<std::pair<Student, Score>> find_best_n_students(std::vector<Student> students, const Exam &exam, size_t n)
{
    std::vector<std::pair<Student, Score>> vec;

    n = std::min(n, students.size());

    std::transform(students.begin(), students.end(), std::back_inserter(vec),
                   [exam](Student student)
                   {
                       return std::make_pair(student, calculate_score(student, exam));
                   });

    std::partial_sort(vec.begin(), vec.begin() + n, vec.end(),
                      [](const std::pair<Student, Score> &first, const std::pair<Student, Score> &second)
                      {
                          return std::greater{}(first.second, second.second);
                      });

    return std::vector<std::pair<Student, Score>>(vec.begin(), vec.begin() + n);
}

// https://codereview.stackexchange.com/a/51182
size_t absoluteDifference(size_t first, size_t second)
{
    return std::max(first, second) - std::min(first, second);
}

size_t max_score_difference(const std::vector<Student> &students, const std::vector<Exam> &exams)
{
    std::vector<size_t> data;

    std::transform(exams.begin(), exams.end(), std::back_inserter(data),
                   [students](const Exam &exam)
                   {
                       std::vector<size_t> scores;
                       std::transform(students.begin(), students.end(), std::back_inserter(scores), [exam](const Student &student)
                                      { return calculate_score(student, exam); });

                       std::sort(scores.begin(), scores.end());
                       std::adjacent_difference(scores.begin(), scores.end(), scores.begin(), absoluteDifference);

                       auto max = std::max_element(
                           // if scores is <= 1, then it will be begin + 0, otherwise begin + 1, to skip first
                           // element in scores
                           scores.begin() + (scores.size() > 1 ? 1 : 0),
                           scores.end());
                       return max == scores.end() ? 0 : *max;
                   });

    auto max = std::max_element(data.begin(),
                                data.end());
    return max == data.end() ? 0 : *max;
}

std::pair<std::unordered_set<Student>, std::unordered_set<Student>> filter_students(const std::vector<Student> &students, const std::vector<Exam> &exams)
{
    // Each student have unordered map of exam + points from them
    // to avoid calling calculate_score 2x
    std::unordered_map<Student, std::unordered_map<std::string, size_t>> data;

    std::transform(students.begin(), students.end(), std::inserter(data, data.begin()),
                   [exams](const Student &student)
                   {
                       std::unordered_map<std::string, size_t> studentExams;

                       std::transform(exams.begin(), exams.end(), std::inserter(studentExams, studentExams.begin()),
                                      [student](const Exam &exam)
                                      {
                                          return std::make_pair(exam.subject, calculate_score(student, exam));
                                      });

                       return std::make_pair(student, studentExams);
                   });

    std::unordered_set<Student> all;

    std::copy_if(students.begin(), students.end(), std::inserter(all, all.begin()),
                 [exams, &data](const Student &student)
                 {
                     return std::all_of(exams.begin(), exams.end(),
                                        [student, &data](const Exam &exam)
                                        {
                                            return data[student][exam.subject] > 100;
                                            // return calculate_score(student, exam) > 100;
                                        });
                 });

    std::unordered_set<Student> some;

    std::copy_if(students.begin(), students.end(), std::inserter(some, some.begin()),
                 [exams, &data](const Student &student)
                 {
                     return std::any_of(exams.begin(), exams.end(),
                                        [student, &data](const Exam &exam)
                                        {
                                            return data[student][exam.subject] > 100;
                                            // return calculate_score(student, exam) > 100;
                                        });
                 });

    return std::make_pair(all, some);
}

Leaderboard get_leaderboard_of_each_subject(const std::vector<Student> &students, const std::vector<Exam> &exams)
{
    std::unordered_map<Student, std::vector<std::pair<Subject, Score>>> data;

    std::transform(students.begin(), students.end(), std::inserter(data, data.begin()),
                   [exams](const Student &student)
                   {
                       std::vector<std::pair<Subject, Score>> examsScores;
                       std::transform(exams.begin(), exams.end(), std::back_inserter(examsScores),
                                      [student](const Exam &exam)
                                      {
                                          return std::make_pair(exam.subject, calculate_score(student, exam));
                                      });
                       return std::make_pair(student, examsScores);
                   });

    Leaderboard leaderboard;

    std::for_each(data.begin(), data.end(),
                  [&leaderboard](const std::pair<Student, std::vector<std::pair<Subject, Score>>> &pair)
                  {
                      std::for_each(pair.second.begin(), pair.second.end(),
                                    [student = pair.first, &leaderboard](const std::pair<Subject, Score> &subjectScore)
                                    {
                                        auto &vec = leaderboard[subjectScore.first];
                                        auto it = std::find_if(vec.begin(), vec.end(),
                                                               [&student](const std::pair<Student, Score> &studentScore)
                                                               {
                                                                   return studentScore.first == student;
                                                               });

                                        if (it == vec.end())
                                        {
                                            vec.push_back(std::make_pair(student, subjectScore.second));
                                        }
                                        else
                                        {
                                            it->second += subjectScore.second;
                                        }
                                    });
                  });

    std::for_each(leaderboard.begin(), leaderboard.end(),
                  [](auto &studentScore)
                  {
                      std::sort(studentScore.second.begin(), studentScore.second.end(),
                                [](const std::pair<Student, Score> &first, const std::pair<Student, Score> &second)
                                {
                                    if (first.second == second.second)
                                    {
                                        return first.first.name > second.first.name;
                                    }
                                    return first.second > second.second;
                                });
                  });

    return leaderboard;
}
