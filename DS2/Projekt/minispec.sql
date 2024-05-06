--Začátek transakce
CREATE OR REPLACE FUNCTION addGroupToUser(int f_groupId) RETURN BOOLEAN AS
    p_group_id INTEGER;
    p_perm_count INTEGER;
BEGIN
    --zjištění maximální váhy skupiny uživatele (skupin s id 1 = nejvyšší)
    SELECT MIN(group_id) INTO p_group_id INTO FROM user_have_group 
    WHERE user_id = vf_currentUserId AND server_id = vf_serverId;

    --pokud je váha požadované skupiny větší než má uživatel (chce přiřadit skupinu s větším oprávněním)
    IF p_group_id < f_groupId THEN
        --zkontrolovat, jestli nemá uživatel oprávnění *, nebo oprávnění na udělování vyšších skupin
        SELECT COUNT(*) INTO p_perm_count FROM permissions WHERE 
            --Uživatel + server se musí rovnat
            user_id = vf_currentUserId AND server_id = vf_serverId AND 
            --permisse musí být * a nesmí být odebrána
            (name = '*' OR name = 'permissions.addhigher') AND remove = 0;

        --pokud tu permissi nemá, nepovedlo se přidat skupinu
        IF p_perm_count = 0 THEN
            RETURN false;
        END IF;
    END IF;

    --vložení skupiny do databáze pro každého uživatele
    FORALL user IN vf_selectedUsers.FIRST..vf_selectedUsers.LAST
        INSERT INTO user_have_group (group_id, user_id, server_id) 
        VALUES (f_groupId, user, vf_serverId);

    RETURN true;
END