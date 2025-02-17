SET SERVEROUTPUT ON;

SELECT * FROM user_tables;

CREATE OR REPLACE PROCEDURE PPrepareTableReward AS
    v_count INTEGER;
BEGIN
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE TABLE_NAME = 'REWARD';
    
    IF v_count > 0 THEN
        EXECUTE IMMEDIATE 'DROP TABLE REWARD';
    END IF;
    
    EXECUTE IMMEDIATE 'CREATE TABLE REWARD (
         id INTEGER PRIMARY KEY,
         student_login CHAR(6) REFERENCES Student,
         winter_reward INTEGER NULL,
         summer_reward INTEGER NULL,
         thesis_reward INTEGER NULL
    )';
END;


CREATE OR REPLACE PROCEDURE PPrepareTableReward2(
    p_name user_tables.TABLE_NAME%TYPE
) AS
    v_count INTEGER;
BEGIN
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE TABLE_NAME = UPPER(p_name);
    
    IF v_count > 0 THEN
        EXECUTE IMMEDIATE 'DROP TABLE ' || p_name;
    END IF;
    
    EXECUTE IMMEDIATE 'CREATE TABLE ' || p_name || ' (
         id INTEGER PRIMARY KEY,
         student_login CHAR(6) REFERENCES Student,
         winter_reward INTEGER NULL,
         summer_reward INTEGER NULL,
         thesis_reward INTEGER NULL
    )';
END;

EXECUTE PPrepareTableReward;


CREATE OR REPLACE PROCEDURE PSetStudentReward(
    p_login STUDENT.LOGIN%TYPE,
    p_rewardType VARCHAR,
    p_reward INTEGER
) AS
    v_id INTEGER;
BEGIN
    SELECT COALESCE(MAX(ID), 0) + 1 INTO v_id FROM REWARD; 
    EXECUTE IMMEDIATE 'INSERT INTO REWARD (id, student_login, ' || p_rewardType || '_reward) VALUES (:id, :login, :reward)' USING v_id, p_login, p_reward;
END;

CREATE OR REPLACE PROCEDURE PSetStudentReward2(
    p_login STUDENT.LOGIN%TYPE,
    p_rewardType VARCHAR,
    p_reward INTEGER,
    p_tableName VARCHAR
) AS
    v_id INTEGER;
BEGIN
    EXECUTE IMMEDIATE 'SELECT COALESCE(MAX(ID), 0) + 1 FROM ' || p_tableName INTO v_id; 
    EXECUTE IMMEDIATE 'INSERT INTO ' || p_tableName || ' (id, student_login, ' || p_rewardType || '_reward) VALUES (:id, :login, :reward)' USING v_id, p_login, p_reward;
END;

EXECUTE PSetStudentReward('min025', 'WINTER', 20);

CREATE OR REPLACE PROCEDURE PUpdateGradeStatic(
    p_grade STUDENT.GRADE%TYPE,
    p_fname STUDENT.FNAME%TYPE,
    p_lname STUDENT.LNAME%TYPE
) AS
BEGIN
    UPDATE STUDENT SET GRADE = p_grade WHERE FNAME = p_fname AND LNAME = p_lname;
END;

CREATE OR REPLACE PROCEDURE PUpdateGradeDynamic(
    p_grade STUDENT.GRADE%TYPE,
    p_fname STUDENT.FNAME%TYPE,
    p_lname STUDENT.LNAME%TYPE
) AS
    v_SQL VARCHAR(1000);
BEGIN
    v_SQL := 'UPDATE STUDENT SET GRADE = ' || p_grade;
    
    IF p_fname <> null AND p_lname <> null THEN
        v_SQL :=  v_SQL || ' WHERE FNAME = ' || p_fname || ' AND LNAME = ' || p_lname;
    ELSIF p_fname <> null THEN
        v_SQL := V_SQL || ' WHERE FNAME = ' || p_fname;
    ELSIF p_lname <> null THEN
        v_SQL := V_SQL || ' WHERE LNAME = ' || p_lname;
    END IF;
    
    EXECUTE IMMEDIATE v_SQL;
END;


CREATE OR REPLACE PROCEDURE PUpdateGrade (
    p_grade STUDENT.GRADE%TYPE, 
    p_fname STUDENT.FNAME%TYPE,
    p_lname STUDENT.LNAME%TYPE,
    p_type VARCHAR
) AS
    v_sql VARCHAR(100);
BEGIN
    v_sql := 'BEGIN PUpdateGrade' || p_type || '(:grade, :fname, :lname); END;';
    DBMS_OUTPUT.PUT_LINE(v_sql);
    EXECUTE IMMEDIATE v_sql USING p_grade, p_fname, p_lname;
END;

EXECUTE PUpdateGrade(2, 'Patrik', 'Mintel', 'Static');


exec sys.kill_locked_sessions('MIN0150');