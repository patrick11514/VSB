SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE checkLog AS
    v_count INTEGER;
    items INTEGER;
BEGIN
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE TABLE_NAME = 'LOG';
    
    IF v_count > 0 THEN
        EXECUTE IMMEDIATE 'SELECT COUNT(*) FROM LOG' INTO items;
        DBMS_OUTPUT.PUT_LINE('V tabulce LOG je ' || items || ' zaznamu.');
        EXECUTE IMMEDIATE 'DROP TABLE LOG';
    END IF;
    
    EXECUTE IMMEDIATE 'CREATE TABLE LOG (
        Id INTEGER,
        Text VARCHAR(1000) NOT NULL
    )';
END;

EXECUTE checkLog();

CREATE OR REPLACE TRIGGER TDeleteOmluva BEFORE DELETE ON OMLUVA FOR EACH ROW
DECLARE
 v_day INTEGER;
 v_exc EXCEPTION;
 v_id INTEGER;
 v_osid OSOBA.ID_OSOBA%TYPE;
 v_poslanec OSOBA%ROWTYPE;
BEGIN
    SELECT EXTRACT(DAY FROM :old.DEN) INTO v_day FROM DUAL;  
    IF MOD(v_day, 2) = 0 THEN
        RAISE v_exc;
    END IF;
    SELECT COALESCE(MAX(Id), 0) INTO v_id FROM LOG;
    SELECT ID_OSOBA INTO v_osid FROM POSLANEC p WHERE p.ID_POSLANEC = :old.ID_POSLANEC;
    SELECT * INTO v_poslanec FROM OSOBA WHERE ID_OSOBA = v_osid;
    
    INSERT INTO LOG (Id, Text) VALUES (v_id + 1, 'Smazana omluva poslance: ' || v_poslanec.JMENO || ' ' || v_poslanec.PRIJMENI || ' ze dne ' || TO_CHAR(:old.DEN, 'DD.MM.YYYY') || ' v ' || TO_CHAR(CURRENT_DATE, 'HH:MI:SS'));
END;

SELECT * FROM LOG;

CREATE OR REPLACE PROCEDURE searchPoslanec(
    p_TEXT VARCHAR
) AS
    v_count INTEGER;
    v_hlasovani INTEGER;
BEGIN
    FOR v_osoba IN (
        SELECT * FROM OSOBA os WHERE os.JMENO LIKE p_TEXT || '%' OR os.PRIJMENI LIKE p_TEXT || '%'
    )
    LOOP
        SELECT COUNT(*) INTO v_count FROM POSLANEC p WHERE p.ID_OSOBA = v_osoba.ID_OSOBA;
        IF v_count > 0 THEN
           
            SELECT COUNT(*) INTO v_hlasovani FROM HLASOVANI_POSLANEC hp 
            INNER JOIN POSLANEC p ON p.ID_POSLANEC = hp.ID_POSLANEC WHERE p.ID_OSOBA = v_osoba.ID_OSOBA;

            DBMS_OUTPUT.PUT_LINE(v_osoba.JMENO || ' ' || v_osoba.PRIJMENI || ' je poslanec, ma ' || v_hlasovani || ' hlasu');
        ELSE
            DBMS_OUTPUT.PUT_LINE(v_osoba.JMENO || ' ' || v_osoba.PRIJMENI || ' neni poslanec');
        END IF;
    END LOOP;
END;

EXECUTE searchPoslanec('Andrej');

SELECT COUNT(*) FROM HLASOVANI_POSLANEC hp 
INNER JOIN POSLANEC p ON p.ID_POSLANEC = hp.ID_POSLANEC
INNER JOIN OSOBA os ON os.ID_OSOBA = p.ID_OSOBA WHERE os.JMENO = 'Andrej' AND os.PRIJMENI = 'Babiš';
