--https://stackoverflow.com/a/6351365
CREATE TYPE NUM_ARRAY AS TABLE OF INTEGER;

CREATE OR REPLACE FUNCTION ADDGROUPTOUSER(f_groupId INTEGER, vf_currentUserId INTEGER, vf_serverId INTEGER, vf_selectedUsers NUM_ARRAY) RETURN BOOLEAN AS
    p_group_id INTEGER;
    p_perm_count INTEGER;
BEGIN
    --zjištění maximální váhy skupiny uživatele (skupin s id 1 = nejvyšší)
    SELECT MIN(group_id) INTO p_group_id FROM user_have_group
    WHERE user_id = vf_currentUserId AND server_id = vf_serverId;

    --pokud je váha požadované skupiny větší než má uživatel (chce přiřadit skupinu s větším oprávněním)
    IF p_group_id < f_groupId OR p_group_id IS NULL THEN
        --zkontrolovat, jestli nemá uživatel oprávnění *, nebo oprávnění na udělování vyšších skupin
        SELECT COUNT(*) INTO p_perm_count FROM permissions WHERE 
            --Uživatel + server se musí rovnat
            user_id = vf_currentUserId AND server_id = vf_serverId AND
            --permisse musí být * a nesmí být odebrána
            (name = '*' OR name = 'permission.group.addhigher') AND remove = 0;

        --pokud tu permissi nemá, nepovedlo se přidat skupinu
        IF p_perm_count = 0 THEN
            RETURN false;
        END IF;
    END IF;

    --vložení skupiny do databáze pro každého uživatele
    FORALL user IN vf_selectedUsers.FIRST..vf_selectedUsers.LAST
        INSERT INTO user_have_group (group_id, user_id, server_id) 
        VALUES (f_groupId, vf_selectedUsers(user), vf_serverId);

    RETURN true;
END;

--https://stackoverflow.com/a/34923712
CREATE OR REPLACE FUNCTION BOOLEAN_TO_CHAR(STATUS IN BOOLEAN)
RETURN VARCHAR2 IS
BEGIN
  RETURN
   CASE STATUS
     WHEN TRUE THEN 'TRUE'
     WHEN FALSE THEN 'FALSE'
     ELSE 'NULL'
   END;
END;

DECLARE
    users NUM_ARRAY;
BEGIN
    users := NUM_ARRAY();
    users.extend(1);
    users(1) := 3; -- XxxHonzikYT_CZ

    --Add Group majitel (1) to XxxHonzikYT_CZ (3) as patrick115 (1) on server Survival (1)
    --True, protože mám dostatečné oprávnění
    DBMS_OUTPUT.PUT_LINE(BOOLEAN_TO_CHAR(ADDGROUPTOUSER(1, 1, 1, users)));

    users(1) := 4; -- Skypad6000

    --Za to Pepa (2) nemá sice dostatečnou skupinu, ale má oprávnění *
    DBMS_OUTPUT.PUT_LINE(BOOLEAN_TO_CHAR(ADDGROUPTOUSER(1, 2, 1, users)));

    --Třeba Vikikle (5) nemá žádné oprávnění, takže přiřazení selže
    DBMS_OUTPUT.PUT_LINE(BOOLEAN_TO_CHAR(ADDGROUPTOUSER(1, 5, 1, users)));

    --Vložení pro více lidí
    users.extend(2);
    users(1) := 4; -- Skypad6000
    users(2) := 5; -- Vikikle
    users(3) := 6; -- Bagetak

    --na serveru Creative (2), kde má oprávnění permissions.group.addhigher, takže opět může přidávat
    DBMS_OUTPUT.PUT_LINE(BOOLEAN_TO_CHAR(ADDGROUPTOUSER(3, 2, 2, users)));
    COMMIT;
END;

--CLEANUP
BEGIN
   DELETE FROM USER_HAVE_GROUP WHERE USER_ID = 3;
   DELETE FROM USER_HAVE_GROUP WHERE USER_ID = 4;
  DELETE FROM USER_HAVE_GROUP WHERE USER_ID = 5;
  DELETE FROM USER_HAVE_GROUP WHERE USER_ID = 6;

   COMMIT;
END;