# Assignment
Implement the functions marked with TODO using STL algorithms, or C++ 20 ranges, it
is up to you. Try to use iterators, algorithms and data structures as much as possible.
Try to avoid "raw loops" (`for`, `while`, `do while`) and `if`/`switch` conditions as much as possible,
only use them when there is no other reasonable way (no applicable STL algorithm).

Watch [this video](https://www.youtube.com/watch?v=2olsGf6JIkU) first before starting to
work on the assignment.

## `find_best_n_students`
Find students that have the best `n` scores on the `exam`.
Use the `calculate_score` function to calculate student's score on a given exam.
Return a vector of `n` pairs (student, score) sorted by score in descending order.
*Avoid ANY cycles and if conditions.*

## `max_score_difference`
For each exam, calculate the scores of each student and sort the scores in descending order.
Then calculate the largest difference between subsequent scores.
For example:
```
scores = [95, 90, 74]
Difference between the first score and the second score is 5.
Difference between the second score and the third score is 16.
The largest difference between subsequent scores is 16.
```

Return the largest difference found amongst all exams.
For example:
```
exam1_scores = [95, 90, 74]     # largest difference = 16
exam2_scores = [98, 90, 88, 40] # largest difference = 48
The largest difference amongst all exams is 48.
```
If there is less than one student or there are no exams, return `0`.

## `filter_students`
Find a set of students that have passed all exams (S1) and a set of students that have passed at least
a single exam (S2). Student passes an exam if his/her score on the exam is at least 100.
Return a pair (S1, S2).

## `get_leaderboard_of_each_subject`
Create a student leaderboard for each subject.
Each exam has an associated subject.
Evaluate all students on all exams and calculate how many total points did each student get for each subject.
Return a leaderboard that maps subjects to a vector of (student, score) pairs sorted in descending order by
score (or name, if score is the same for two students).
