SELECT c.CODE, c.NAME
FROM COURSE c
INNER JOIN TEACHER t
    ON c.TEACHER_LOGIN = t.LOGIN
WHERE t.FNAME = 'Jerry' AND t.LNAME = 'Jordon';

SELECT distinct s.fname, s.lname 
FROM STUDENT s
INNER JOIN studentcourse sc
    ON s.login = sc.student_login
INNER JOIN course c
    ON c.code = sc.course_code
INNER JOIN teacher t
    ON t.login = c.teacher_login
WHERE t.fname = 'Jerry' AND
    t.lname = 'Jordon' AND
    sc.year IN (2020, 2021)
ORDER BY s.lname;

SELECT s.fname, s.lname, (
    SELECT COUNT(*) FROM studentcourse sc WHERE sc.student_login = s.login
) COURSE_COUNT FROM student s WHERE s.email LIKE '%@vsb.cz';

INSERT INTO teacher (fname, lname, login, department)
VALUES ('Petr', 'Burton', 'bur154', 'Department of Mathematics');

UPDATE course SET course.teacher_login = 'bur154' WHERE NAME = 'Database Systems II';

DELETE FROM studentcourse WHERE course_code IN (
    SELECT c.code FROM course c
    INNER JOIN teacher t
    ON c.teacher_login = t.login
    WHERE t.fname = 'Carl' AND t.lname = 'Artis'
);

INSERT INTO studentcourse (couse_code, year, student_login)
SELECT c.code, 2021, (
    SELECT login FROM student WHERE fname = 'Barbara' AND lname = 'Jones'
) FROM course c
INNER JOIN teacher t
    ON c.teac.teacher_login=t.login
WHERE t.fname = 'Walter' AND t.lname = 'Perryman';

UPDATE student SET student.date_of_birth = TO_DATE('02.03.1997', 'DD.MM.YYYY') WHERE student.login = 'smi324';

SELECT s.fname, s.lname, FLOOR(MONTHS_BETWEEN(s.date_of_birth, CURRENT_DATE) * 12) FROM student s;

SELECT lname, date_of_birth, ADD_MONTHS(date_of_birth, 12 * 18) AGE18
FROM student;

SELECT lname, date_of_birth, 
EXTRACT(DAY FROM date_of_birth) D,
EXTRACT(MONTH FROM date_of_birth) M,
EXTRACT(YEAR FROM date_of_birth) Y
FROM student;

SELECT * FROM student s1
JOIN student s2 ON ADD_MONTHS(s1.date_of_birth, 3) >= s2.date_of_birth OR ADD_MONTHS(s1.date_of_birth, -3) <= s2.date_of_birth
WHERE s1.fname = 'Michael' AND s1.lname = 'Silverman';

SELECT s1.fname, s2.lname FROM student s1
INNER JOIN student s1
ON 1=1
WHERE s1.fname = 'Michael' AND s1.lname = 'Silverman' AND s1.date_of_birth BETWEEN ADD_MONTHS(s1.date_of_birth, -3) AND ADD_MONTHS(s1.date_of_birth, 3);

SELECT UPPER(fname || ' ' || lname) FROM teacher;

SELECT lname, LENGTH(lname) FROM teacher;

SELECT REPLACE(name, ' ', '') FROM course;

SELECT LPAD(CAPACITY, 3, '0') FROM course;

SELECT SUBSTR(code, 5, 3) FROM course;

--nebo bez druhého parametru
SELECT SUBSTR(name, 3, LENGTH(name) - 2) FROM course;

SELECT name, INSTR(name, ' ') FROM course;