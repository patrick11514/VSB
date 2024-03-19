SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE PAddStudentToCourse (
    p_student_login STUDENT.LOGIN%TYPE,
    p_course_code COURSE.CODE%TYPE,
    p_year STUDENTCOURSE.YEAR%TYPE
) AS
    v_capacity COURSE.CAPACITY%TYPE;
    v_student_count INTEGER;
BEGIN
    SELECT CAPACITY INTO v_capacity FROM COURSE WHERE CODE = p_course_code;
    SELECT COUNT(*) INTO v_student_count FROM STUDENTCOURSE WHERE COURSE_CODE = p_course_code AND YEAR = p_year;
    
    IF v_capacity > v_student_count THEN
        INSERT INTO STUDENTCOURSE (STUDENT_LOGIN, COURSE_CODE, YEAR) VALUES (p_student_login, p_course_code, p_year);
    ELSE
        dbms_output.put_line('Kurz je již plnì obsazen');
    END IF;
END;

CREATE OR REPLACE TRIGGER TInsertStudentCourse BEFORE INSERT ON STUDENTCOURSE FOR EACH ROW
DECLARE
    v_capacity COURSE.CAPACITY%TYPE;
    v_student_count INTEGER;
    v_ex EXCEPTION;
BEGIN
    SELECT CAPACITY INTO v_capacity FROM COURSE WHERE CODE = :new.COURSE_CODE;
    SELECT COUNT(*) INTO v_student_count FROM STUDENTCOURSE WHERE COURSE_CODE = :new.COURSE_CODE AND YEAR = :new.YEAR;
    
    IF v_capacity <= v_student_count THEN
        DBMS_OUTPUT.PUT_LINE('Kapacita kurzu byla pøekroèena');
        RAISE v_ex;
    END IF;
END;

CREATE OR REPLACE FUNCTION FGetLogin(p_lname Student.lname%TYPE) RETURN Student.login%TYPE AS
    v_studentCount INT;
BEGIN   
    SELECT COUNT(*) + 1 INTO v_studentCount
    FROM Student;

    RETURN LOWER(SUBSTR(p_lname, 1, 3)) || LPAD(v_studentCount, 3, '0');
END;

-- 2.4
CREATE OR REPLACE FUNCTION FAddStudent4(p_fname Student.fname%TYPE, p_lname Student.lname%TYPE,
    p_email Student.email%TYPE, p_grade Student.grade%TYPE, p_dateOfBirth Student.date_of_birth%TYPE) RETURN VARCHAR AS
    
    v_login Student.login%TYPE;
    v_count INTEGER;
BEGIN
    
    SELECT COUNT(*) INTO v_count FROM STUDENT WHERE GRADE = p_grade;
    
    if p_grade = 1 AND v_count >= 20 THEN
        RETURN 'FULL';
    ELSIF p_grade = 2 AND v_count >= 15 THEN
        RETURN 'FULL';
    ELSIF v_count >= 10 THEN
        RETURN 'FULL';
    END IF;
    
    v_login := FGetNextLogin(p_lname);
    
    INSERT INTO Student (login, fname, lname, email, grade, date_of_birth)
    VALUES (v_login, p_fname, p_lname, p_email, p_grade, p_dateOfBirth);

    RETURN v_login;
    
EXCEPTION
    WHEN OTHERS THEN
        RETURN 'error';
END;


CREATE OR REPLACE FUNCTION FLoginExists(
    p_login STUDENT.LOGIN%TYPE
) RETURN BOOLEAN AS
    v_count INTEGER;
BEGIN
    SELECT COUNT(*) INTO v_count FROM STUDENT WHERE LOGIN = p_login;
    IF v_count > 0 THEN
        RETURN true;
    END IF;
    RETURN false;
END;


CREATE OR REPLACE FUNCTION FGetNextLogin(
    p_lname STUDENT.LNAME%TYPE
) RETURN STUDENT.LOGIN%TYPE AS
    v_prefix VARCHAR(3);
    v_number INTEGER;
    v_login STUDENT.LOGIN%TYPE;
BEGIN
    v_prefix := LOWER(SUBSTR(p_lname, 1, 3));
    v_number := 0;
    
    LOOP
        v_login := v_prefix || LPAD(v_number, 3, '0');
    
        EXIT WHEN NOT FLoginExists(v_login);
        --IF NOT FLoginExists(v_login) THEN
        --    RETURN v_login;
        --END IF;
        v_number := v_number + 1;
    END LOOP;
    
    RETURN v_login;
END;

exec sys.kill_locked_sessions('MIN0150');

BEGIN
    DBMS_OUTPUT.PUT_LINE(FAddStudent4('Jan', 'Gajdos', 'jan@vsb.cz', 4, CURRENT_DATE));
END;

DECLARE
    CURSOR c_student IS SELECT FNAME, LNAME FROM STUDENT;
    v_fname STUDENT.FNAME%TYPE;
    v_lname STUDENT.LNAME%TYPE;
BEGIN
    OPEN c_student;
    
    LOOP
        EXIT WHEN c_student%NOTFOUND;
        FETCH c_student INTO v_fname, v_lname;
        DBMS_OUTPUT.PUT_LINE(v_fname || ' ' || v_lname);
    END LOOP;
    
    CLOSE c_student;
END;

BEGIN
    FOR v_student IN (
        SELECT FNAME, LNAME FROM STUDENT
    )
    LOOP
         DBMS_OUTPUT.PUT_LINE(v_student.FNAME || ' ' || v_student.LNAME);
    END LOOP;
END;