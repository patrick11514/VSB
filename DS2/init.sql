CREATE TABLE Student
(
    login CHAR(6) CONSTRAINT Student_PK PRIMARY KEY,
    fname VARCHAR(30) NOT NULL,
    lname VARCHAR(50) NOT NULL,
    email VARCHAR(50) NOT NULL,
    grade INTEGER  NOT NULL,
    date_of_birth DATE NOT NULL
);

CREATE TABLE Teacher
(
    login CHAR(6) CONSTRAINT Teacher_PK PRIMARY KEY,
    fname VARCHAR(30) NOT NULL,
    lname VARCHAR(50) NOT NULL,
    department VARCHAR(50) NOT NULL
);

CREATE TABLE Course
(
    code CHAR(11) PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    capacity INT NOT NULL,
    teacher_login CHAR(6),
    CONSTRAINT Course_Teacher_FK FOREIGN KEY (teacher_login) REFERENCES Teacher(login)
);

CREATE TABLE StudentCourse
(
    student_login CHAR(6),
    course_code CHAR(11),
    year INT,
    CONSTRAINT StudentCourse_PK PRIMARY KEY (student_login, course_code, year),
    CONSTRAINT StudentCourse_Student_FK FOREIGN KEY (student_login) REFERENCES Student(login),
    CONSTRAINT StudentCourse_Course_FK FOREIGN KEY (course_code) REFERENCES Course(code)
);

insert into Student (login, fname, lname, email, grade, date_of_birth) values ('smi324', 'Mary', 'Smith', 'smith@gmail.com', 1, to_date('1998/05/31', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('joh357', 'Patricia', 'Johnson', 'patricia.johnson@vsb.cz', 1, to_date('1997/09/23', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('wil986', 'Linda', 'Williams', 'williams.linda@tieto.com', 1, to_date('1998/02/02', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('jon067', 'Barbara', 'Jones', 'jones.barbara@tieto.com', 1, to_date('1997/12/12', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('bro654', 'Elizabeth', 'Brown', 'brown.elizabeth@tieto.com', 1, to_date('1998/06/06', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('gan098', 'James', 'Gannon', 'jimmyg@gmail.com', 1, to_date('1998/04/22', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('far483', 'John', 'Farnsworth', 'farnie@yahoo.com', 1, to_date('1998/04/02', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('bau764', 'Robert', 'Baughman', 'robert.baughman@vsb.cz', 2, to_date('1997/02/25', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('sil012', 'Michael', 'Silverman', 'silverman@gmail.com', 2, to_date('1997/03/15', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('sat767', 'William', 'Satterfield', 'satter@hotmail.com', 2, to_date('1996/11/11', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('rol087', 'David', 'Royal', 'david.royal@vsb.cz', 2, to_date('1996/10/12', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('mcc676', 'Richard', 'McCrary', 'mccrary@dell.com', 2, to_date('1996/12/20', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('kow007', 'Charles', 'Kowalski', 'charliw@yahoo.com', 3, to_date('1995/05/12', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('joy087', 'Joseph', 'Joy', 'joy@oracle@com', 3, to_date('1995/01/04', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('gri453', 'Thomas', 'Grigsby', 'thomas.grigsby@vsb.cz', 3, to_date('1994/09/27', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('gre227', 'Christopher', 'Greco', 'greco.gmail.com', 3, to_date('1994/11/07', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('cab466', 'Daniel', 'Cabral', 'daniel.cabral@vsb.cz', 4, to_date('1994/05/05', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('tro552', 'Paul', 'Trout', 'paul.trout@vsb.cz', 4, to_date('1993/11/29', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('rin766', 'Mark', 'Rinehard', 'rino@yahoo.com', 5, to_date('1992/04/10', 'yyyy/mm/dd'));
insert into Student (login, fname, lname, email, grade, date_of_birth) values ('pen048', 'Erika', 'Pena', 'erika.pena@vsb.cz', 5, to_date('1992/02/22', 'yyyy/mm/dd'));


insert into Teacher (login, fname, lname, department) values ('jor012', 'Jerry', 'Jordon', 'Department of Computer Science');
insert into Teacher (login, fname, lname, department) values ('gil233', 'Dennis', 'Gilman', 'Department of Computer Science');
insert into Teacher (login, fname, lname, department) values ('per085', 'Walter', 'Perryman', 'Department of Computer Science');
insert into Teacher (login, fname, lname, department) values ('new056', 'Patrick', 'Newsom', 'Department of Mathematic');
insert into Teacher (login, fname, lname, department) values ('men623', 'Peter', 'Menard', 'Department of Mathematic');
insert into Teacher (login, fname, lname, department) values ('mar118', 'Harold', 'Martino', 'Department of Management');
insert into Teacher (login, fname, lname, department) values ('gra198', 'Douglas', 'Graf', 'Department of Management');
insert into Teacher (login, fname, lname, department) values ('art035', 'Carl', 'Artis', 'Department of Management');


insert into Course (code, name, capacity, teacher_login) values ('460-ds1-011', 'Database Systems I', 5, 'jor012');
insert into Course (code, name, capacity, teacher_login) values ('460-ds2-012', 'Database Systems II', 5, 'jor012');
insert into Course (code, name, capacity, teacher_login) values ('460-cns-922', 'Computer Networks', 10, 'jor012');
insert into Course (code, name, capacity, teacher_login) values ('460-img-101', 'Image Processing', 5, 'jor012');
insert into Course (code, name, capacity, teacher_login) values ('460-jav-226', 'Java Technologies', 8, 'per085');
insert into Course (code, name, capacity, teacher_login) values ('460-pyt-001', 'Python', 10, 'per085');
insert into Course (code, name, capacity, teacher_login) values ('460-web-654', 'Web Technologies', 15, 'per085');
insert into Course (code, name, capacity, teacher_login) values ('470-alg-220', 'Algebra', 15, 'new056');
insert into Course (code, name, capacity, teacher_login) values ('470-dis-902', 'Discrete Math', 15, 'new056');
insert into Course (code, name, capacity, teacher_login) values ('420-mic-061', 'Microeconomics', 8, 'art035');
insert into Course (code, name, capacity, teacher_login) values ('420-mac-022', 'Macroeconomics', 8, 'art035');
insert into Course (code, name, capacity, teacher_login) values ('420-man-111', 'Management', 8, 'art035');
insert into Course (code, name, capacity, teacher_login) values ('420-toe-166', 'Theory of Economy', 15, 'art035');
insert into Course (code, name, capacity, teacher_login) values ('420-adv-061', 'Advertisement', 8, 'art035');

insert into StudentCourse (student_login, course_code, year) values ('smi324', '420-mic-061', 2020);
insert into StudentCourse (student_login, course_code, year) values ('smi324', '420-mac-022', 2020);
insert into StudentCourse (student_login, course_code, year) values ('smi324', '420-man-111', 2020);
insert into StudentCourse (student_login, course_code, year) values ('smi324', '420-toe-166', 2020);
insert into StudentCourse (student_login, course_code, year) values ('smi324', '420-adv-061', 2020);
insert into StudentCourse (student_login, course_code, year) values ('joh357', '420-mic-061', 2020);
insert into StudentCourse (student_login, course_code, year) values ('joh357', '420-mac-022', 2020);
insert into StudentCourse (student_login, course_code, year) values ('joh357', '420-man-111', 2020);
insert into StudentCourse (student_login, course_code, year) values ('wil986', '420-mic-061', 2020);
insert into StudentCourse (student_login, course_code, year) values ('wil986', '420-mac-022', 2020);
insert into StudentCourse (student_login, course_code, year) values ('jon067', '420-mic-061', 2020);
insert into StudentCourse (student_login, course_code, year) values ('jon067', '420-mac-022', 2020);
insert into StudentCourse (student_login, course_code, year) values ('jon067', '420-man-111', 2020);
insert into StudentCourse (student_login, course_code, year) values ('bro654', '420-mic-061', 2020);
insert into StudentCourse (student_login, course_code, year) values ('bro654', '420-mac-022', 2020);
insert into StudentCourse (student_login, course_code, year) values ('bro654', '420-man-111', 2020);
insert into StudentCourse (student_login, course_code, year) values ('gan098', '470-dis-902', 2020);
insert into StudentCourse (student_login, course_code, year) values ('far483', '470-dis-902', 2020);
insert into StudentCourse (student_login, course_code, year) values ('bau764', '460-ds1-011', 2020);
insert into StudentCourse (student_login, course_code, year) values ('sil012', '460-ds1-011', 2020);
insert into StudentCourse (student_login, course_code, year) values ('rol087', '460-ds1-011', 2020);
insert into StudentCourse (student_login, course_code, year) values ('mcc676', '470-alg-220', 2020);
insert into StudentCourse (student_login, course_code, year) values ('mcc676', '470-dis-902', 2020);
insert into StudentCourse (student_login, course_code, year) values ('kow007', '470-dis-902', 2020);
insert into StudentCourse (student_login, course_code, year) values ('gri453', '460-img-101', 2020);
insert into StudentCourse (student_login, course_code, year) values ('gre227', '460-img-101', 2020);
insert into StudentCourse (student_login, course_code, year) values ('cab466', '460-img-101', 2020);
insert into StudentCourse (student_login, course_code, year) values ('cab466', '460-jav-226', 2020);
insert into StudentCourse (student_login, course_code, year) values ('tro552', '460-img-101', 2020);
insert into StudentCourse (student_login, course_code, year) values ('tro552', '460-web-654', 2020);
insert into StudentCourse (student_login, course_code, year) values ('rin766', '460-web-654', 2020);
insert into StudentCourse (student_login, course_code, year) values ('smi324', '420-toe-166', 2021);
insert into StudentCourse (student_login, course_code, year) values ('smi324', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('smi324', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('joh357', '420-toe-166', 2021);
insert into StudentCourse (student_login, course_code, year) values ('joh357', '420-adv-061', 2021);
insert into StudentCourse (student_login, course_code, year) values ('joh357', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('joh357', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('wil986', '420-man-111', 2021);
insert into StudentCourse (student_login, course_code, year) values ('wil986', '420-toe-166', 2021);
insert into StudentCourse (student_login, course_code, year) values ('wil986', '420-adv-061', 2021);
insert into StudentCourse (student_login, course_code, year) values ('wil986', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('jon067', '420-man-111', 2021);
insert into StudentCourse (student_login, course_code, year) values ('jon067', '420-toe-166', 2021);
insert into StudentCourse (student_login, course_code, year) values ('bro654', '420-toe-166', 2021);
insert into StudentCourse (student_login, course_code, year) values ('bro654', '420-adv-061', 2021);
insert into StudentCourse (student_login, course_code, year) values ('bro654', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('bro654', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gan098', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gan098', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gan098', '460-web-654', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gan098', '420-toe-166', 2021);
insert into StudentCourse (student_login, course_code, year) values ('far483', '460-web-654', 2021);
insert into StudentCourse (student_login, course_code, year) values ('bau764', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('bau764', '460-ds2-012', 2021);
insert into StudentCourse (student_login, course_code, year) values ('sil012', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('sil012', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('sil012', '460-ds2-012', 2021);
insert into StudentCourse (student_login, course_code, year) values ('sat767', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('sat767', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('sat767', '460-web-654', 2021);
insert into StudentCourse (student_login, course_code, year) values ('mcc676', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('mcc676', '460-cns-922', 2021);
insert into StudentCourse (student_login, course_code, year) values ('mcc676', '460-web-654', 2021);
insert into StudentCourse (student_login, course_code, year) values ('mcc676', '420-toe-166', 2021);
insert into StudentCourse (student_login, course_code, year) values ('joy087', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('joy087', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('joy087', '460-cns-922', 2021);
insert into StudentCourse (student_login, course_code, year) values ('joy087', '460-web-654', 2021);
insert into StudentCourse (student_login, course_code, year) values ('joy087', '420-toe-166', 2021);
insert into StudentCourse (student_login, course_code, year) values ('joy087', '420-man-111', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gri453', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gri453', '460-cns-922', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gri453', '460-img-101', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gri453', '460-jav-226', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gri453', '460-web-654', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gri453', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gre227', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gre227', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gre227', '460-cns-922', 2021);
insert into StudentCourse (student_login, course_code, year) values ('gre227', '460-web-654', 2021);
insert into StudentCourse (student_login, course_code, year) values ('cab466', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('cab466', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('cab466', '460-cns-922', 2021);
insert into StudentCourse (student_login, course_code, year) values ('cab466', '460-jav-226', 2021);
insert into StudentCourse (student_login, course_code, year) values ('cab466', '460-web-654', 2021);
insert into StudentCourse (student_login, course_code, year) values ('tro552', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('tro552', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('tro552', '460-cns-922', 2021);
insert into StudentCourse (student_login, course_code, year) values ('tro552', '460-img-101', 2021);
insert into StudentCourse (student_login, course_code, year) values ('tro552', '460-jav-226', 2021);
insert into StudentCourse (student_login, course_code, year) values ('tro552', '460-web-654', 2021);
insert into StudentCourse (student_login, course_code, year) values ('rin766', '470-dis-902', 2021);
insert into StudentCourse (student_login, course_code, year) values ('rin766', '460-cns-922', 2021);
insert into StudentCourse (student_login, course_code, year) values ('rin766', '460-img-101', 2021);
insert into StudentCourse (student_login, course_code, year) values ('rin766', '460-jav-226', 2021);
insert into StudentCourse (student_login, course_code, year) values ('rin766', '460-web-654', 2021);
insert into StudentCourse (student_login, course_code, year) values ('rin766', '470-alg-220', 2021);
insert into StudentCourse (student_login, course_code, year) values ('rin766', '420-toe-166', 2021);
insert into StudentCourse (student_login, course_code, year) values ('rin766', '420-man-111', 2021);
