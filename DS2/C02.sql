SET SERVEROUTPUT ON;

CREATE PROCEDURE PPRINT(p_text VARCHAR) AS
--DECLARE
--    v_text STUDENT.LOGIN%TYPE; //VARCHAR(20);
BEGIN
    
    --v_text := 'A';
    
    --v_text := v_text || 'B'; 
    
    dbms_output.put_line(p_text);
END;

EXECUTE PPRINT('Ahoj');

CREATE OR REPLACE PROCEDURE PAddStudent1(
    p_login STUDENT.LOGIN%TYPE, 
    p_fname STUDENT.FNAME%TYPE, 
    p_lname STUDENT.LNAME%TYPE, 
    p_email STUDENT.EMAIL%TYPE,
    p_grade STUDENT.GRADE%TYPE, 
    p_dateOfBirth STUDENT.DATE_OF_BIRTH%TYPE
) AS
BEGIN
    INSERT INTO STUDENT (LOGIN, FNAME, LNAME, EMAIL, GRADE, DATE_OF_BIRTH) VALUES (p_login, p_fname, p_lname, p_email, p_grade, p_dateOfBirth);
END;

EXECUTE PAddStudent1('min150', 'Patrik', 'Mintel', 'ja@patrick115.eu', 1, TO_DATE('01.05.2003', 'DD.MM.YYYY'));

CREATE OR REPLACE PROCEDURE PAddStudent2(
    p_fname STUDENT.FNAME%TYPE, 
    p_lname STUDENT.LNAME%TYPE, 
    p_email STUDENT.EMAIL%TYPE,
    p_grade STUDENT.GRADE%TYPE, 
    p_dateOfBirth STUDENT.DATE_OF_BIRTH%TYPE
) AS

v_login STUDENT.LOGIN%TYPE;

BEGIN
    v_login := LOWER(SUBSTR(p_lname, 1, 3)) || '000';
    
    INSERT INTO STUDENT (LOGIN, FNAME, LNAME, EMAIL, GRADE, DATE_OF_BIRTH) VALUES (v_login, p_fname, p_lname, p_email, p_grade, p_dateOfBirth);
END;

EXECUTE PAddStudent2('Patrik', 'Mintel', 'ja@patrick115.eu', 1, TO_DATE('01.05.2003', 'DD.MM.YYYY'));


CREATE OR REPLACE PROCEDURE PAddStudent3(
    p_fname STUDENT.FNAME%TYPE, 
    p_lname STUDENT.LNAME%TYPE, 
    p_email STUDENT.EMAIL%TYPE,
    p_grade STUDENT.GRADE%TYPE, 
    p_dateOfBirth STUDENT.DATE_OF_BIRTH%TYPE
) AS

v_login STUDENT.LOGIN%TYPE;
v_count INTEGER;
v_fname STUDENT.FNAME%TYPE;

BEGIN
    SELECT COUNT(*) INTO v_count FROM STUDENT;

    v_login := LOWER(SUBSTR(p_lname, 1, 3)) || LPAD(v_count + 1, 3, '0');
    
    INSERT INTO STUDENT (LOGIN, FNAME, LNAME, EMAIL, GRADE, DATE_OF_BIRTH) VALUES (v_login, p_fname, p_lname, p_email, p_grade, p_dateOfBirth);
    
    SELECT LOGIN, FNAME INTO v_login, v_fname FROM STUDENT WHERE LOGIN = v_login;
    
    DBMS_OUTPUT.PUT_LINE('Login: ' || v_login || ', Jmeno: ' || v_fname);
    
    COMMIT;
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('Záznam nebyl nalezen');
        ROLLBACK;
    WHEN TOO_MANY_ROWS THEN
        DBMS_OUTPUT.PUT_LINE('Pøíliš mnoho záznamù');
        ROLLBACK;
END;

EXECUTE PAddStudent3('Patrik', 'Mintel', 'ja@patrick115.eu', 1, TO_DATE('01.05.2003', 'DD.MM.YYYY'));

CREATE OR REPLACE FUNCTION FAddStudent(
    p_fname STUDENT.FNAME%TYPE, 
    p_lname STUDENT.LNAME%TYPE, 
    p_email STUDENT.EMAIL%TYPE,
    p_grade STUDENT.GRADE%TYPE, 
    p_dateOfBirth STUDENT.DATE_OF_BIRTH%TYPE
) RETURN STUDENT.LOGIN%TYPE AS

v_login STUDENT.LOGIN%TYPE;
v_count INTEGER;
v_fname STUDENT.FNAME%TYPE;
--v_student STUDENT%ROWTYPE;

BEGIN
    v_login := FGetLogin(p_lname);
    
    INSERT INTO STUDENT (LOGIN, FNAME, LNAME, EMAIL, GRADE, DATE_OF_BIRTH) VALUES (v_login, p_fname, p_lname, p_email, p_grade, p_dateOfBirth);
    
    SELECT LOGIN, FNAME INTO v_login, v_fname FROM STUDENT WHERE LOGIN = v_login;
    --SELECT * INTO v_student FROM STUDENT WHERE LOGIN = v_login;
    DBMS_OUTPUT.PUT_LINE('Login: ' || v_login || ', Jmeno: ' || v_fname);
    
    RETURN v_login;
END;

EXECUTE PPRINT(FAddStudent('Patrik', 'Mintel', 'ja@patrick115.eu', 1, TO_DATE('01.05.2003', 'DD.MM.YYYY')));

SELECT FNAME, FAddStudent('Patrik', 'Mintel', 'ja@patrick115.eu', 1, TO_DATE('01.05.2003', 'DD.MM.YYYY')) FROM STUDENT;

CREATE OR REPLACE FUNCTION FGetLogin(
p_lname STUDENT.LNAME%TYPE
) RETURN STUDENT.LOGIN%TYPE AS 

    v_login STUDENT.LOGIN%TYPE;
    v_count INTEGER;
    
BEGIN
    SELECT COUNT(*) INTO v_count FROM STUDENT;

    v_login := LOWER(SUBSTR(p_lname, 1, 3)) || LPAD(v_count + 1, 3, '0');
    
    RETURN v_login;
END;

EXECUTE PPRINT(FGetLogin('Mintìl'));
SELECT FNAME, FGetLogin(LNAME) FROM STUDENT;

CREATE OR REPLACE TRIGGER TModifyInsertStudent BEFORE INSERT ON STUDENT FOR EACH ROW
DECLARE
    v_myEx ECEPTION;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Nový student: ' || :new.LOGIN || ' ' || :new.FNAME);
    :new.FNAME := 'XXX';
    
    RAISE v_myEx;
END;

CREATE OR REPLACE TRIGGER TInsertStudent AFTER INSERT ON STUDENT FOR EACH ROW
BEGIN
    DBMS_OUTPUT.PUT_LINE('Nový student: ' || :new.LOGIN || ' ' || :new.FNAME);
END;

CREATE OR REPLACE TRIGGER TDeleteStudent AFTER DELETE ON STUDENT FOR EACH ROW
BEGIN
    DBMS_OUTPUT.PUT_LINE('Starý student: ' || :old.LOGIN || ' ' || :old.FNAME);
END;

CREATE OR REPLACE TRIGGER TUpdateStudent AFTER UPDATE ON STUDENT FOR EACH ROW
BEGIN
    DBMS_OUTPUT.PUT_LINE('Nový student: ' || :new.LOGIN || ' ' || :new.FNAME);
    DBMS_OUTPUT.PUT_LINE('Starý student: ' || :old.LOGIN || ' ' || :old.FNAME);
END;

EXECUTE PPRINT(FAddStudent('Patrik', 'Molcik', 'ja@patrick115.eu', 1, TO_DATE('01.05.2003', 'DD.MM.YYYY')));

DELETE FROM STUDENT WHERE login = 'min150';

UPDATE STUDENT SET FNAME = 'Tom' WHERE login = 'min000';

COMMIT;
