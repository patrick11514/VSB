/*1;1;100;Vyberte všechny tickety, seřazené od nejnovějšíhé*/
SELECT t.ticket_id, t.name, t.author, t.moderator, t.created_at
FROM tickets t
ORDER BY t.created_at DESC

/*1;2;780;Seřaďte všechny zprávy podle roku, měsíce a dne*/
SELECT m.message_id, m.ticket_id, m.author, m.created_at
FROM messages m
ORDER BY YEAR(m.created_at) DESC, MONTH(m.created_at) DESC, DAY(m.created_at) DESC

/*1;3;100;Vypište tickety seřazené podle délky jejich názvu*/
SELECT t.ticket_id, t.name, LEN(t.name) AS name_length, t.author, t.moderator, t.created_at
FROM tickets t
ORDER BY name_length DESC

/*1;4;294;Seřaďte poznámky od nejstarších*/
SELECT c.comment_id, c.ticket_id, c.moderator, c.commented_at, c.comment
FROM comments c
ORDER BY c.commented_at ASC

/*2;1;11;Vypište všechny tickety, které mají v názvu slovo problém*/
SELECT t.ticket_id, t.name, t.author, t.created_at
FROM tickets t
WHERE t.name LIKE '%problém%'

/*2;2;35;Vypište všechny tickety, které mají název delší než 40 znaků, nebo nejsou napsány v roce 2024*/
SELECT t.ticket_id, t.name, t.author, t.created_at
FROM tickets t
WHERE LEN(t.name) > 40 OR NOT YEAR(t.created_at) = 2023

/*2;3;30;Vypište zprávy, které mají zároveň více než 250 znaků a jsou napsány v roce 2024 nebo mají přesně 400 znaků*/
SELECT m.message_id, m.ticket_id, m.author, m.created_at, m.message
FROM messages m
WHERE (LEN(m.message) > 250 AND YEAR(m.created_at) = 2024) OR LEN(m.message) = 400

/*2;4;5;Vypište všechny tickety, které mají v názvu slovo chyba a zároveň nejsou napsány v roce 2024*/
SELECT t.ticket_id, t.name, t.author, t.created_at
FROM tickets t
WHERE t.name LIKE '%chyba%' AND NOT YEAR(t.created_at) = 2024

/*3;1;1;Vypište id moderátorů, kteří nebyli přiřazeni k žádnému ticketu, za pomocí EXISTS*/
SELECT u.user_id
FROM users u
WHERE u.role = 'M' AND NOT EXISTS (
    SELECT *
    FROM tickets t
    WHERE t.moderator = u.user_id
)

/*3;2;1;Vypište id moderátorů, kteří nebyli přiřazeni k žádnému ticketu, za pomocí IN*/
SELECT u.user_id
FROM users u
WHERE u.role = 'M' AND u.user_id NOT IN (
    SELECT t.moderator
    FROM tickets t
)

/*3;3;1;Vypište id moderátorů, kteří nebyli přiřazeni k žádnému ticketu, za pomocí ALL*/
SELECT u.user_id
FROM users u
WHERE u.role = 'M' AND u.user_id != ALL (
    SELECT t.moderator
    FROM tickets t
)

/*3;4;1;Vypište id moderátorů, kteří nebyli přiřazeni k žádnému ticketu, za pomocí EXCEPT*/
SELECT u.user_id
FROM users u
WHERE u.role = 'M'
EXCEPT
SELECT t.moderator
FROM tickets t

/*4;1;100;Vypočítejte průměrnou délku zprávy u daného ticketu*/
SELECT m.ticket_id, AVG(CAST(LEN(m.message) AS FLOAT)) AS avg_message_length
FROM messages m
GROUP BY m.ticket_id
ORDER BY m.ticket_id

/*4;2;20;Vraťte největší délku poznámky pro tickety, které mají více jak 4 poznámky a seřaďte je od nejdelší*/
SELECT c.ticket_id, MAX(LEN(c.comment)) AS max_comment_length
FROM comments c
GROUP BY c.ticket_id
HAVING COUNT(c.comment_id) > 4
ORDER BY max_comment_length DESC

/*4;3;6;Spočítejte, kolik uživatelů má email u jednotlivých domén*/
SELECT SUBSTRING(u.email, CHARINDEX('@', u.email) + 1, LEN(u.email)) AS domain, COUNT(u.user_id) AS users_count
FROM users u
GROUP BY SUBSTRING(u.email, CHARINDEX('@', u.email) + 1, LEN(u.email))

/*4;4;1;Spočítejte, kolik ticketů bylo založeno v březnu daného roku*/
SELECT YEAR(t.created_at) AS year, COUNT(t.ticket_id) AS tickets_count
FROM tickets t
WHERE MONTH(t.created_at) = 3
GROUP BY YEAR(t.created_at)

/*5;1;9;Vypište zprávy, které jsou napsány v roce 2024, ale jsou součástí ticketu vytvořeného v roce 2023 (JOIN)*/
SELECT m.message_id, m.message
FROM messages m
JOIN tickets t ON m.ticket_id = t.ticket_id
WHERE YEAR(m.created_at) = 2024 AND YEAR(t.created_at) = 2023

/*5;2;9;Vypište zprávy, které jsou napsány v roce 2024, ale jsou součástí ticketu vytvořeného v roce 2023 (IN)*/
SELECT m.message_id, m.message
FROM messages m
WHERE YEAR(m.created_at) = 2024 AND m.ticket_id IN (
    SELECT t.ticket_id
    FROM tickets t
    WHERE YEAR(t.created_at) = 2023
)

/*5;3;100;Vypište počet zpráv, které má jednotlivý ticket*/
SELECT t.ticket_id, COUNT(m.message_id) AS messages_count
FROM tickets t
LEFT JOIN messages m ON t.ticket_id = m.ticket_id
GROUP BY t.ticket_id

/*5;4;30;Vypište počet ticketů, ke kterým jsou přiřazeni jednotliví moderátoři*/
SELECT u.user_id, u.username, COUNT(t.ticket_id) AS tickets_count
FROM users u
LEFT JOIN tickets t ON u.user_id = t.moderator
WHERE u.role = 'M'
GROUP BY u.user_id, u.username;

/*6;1;94;U každého uživatele nalezněte ticket, ve kterém je největší průměrná délka zprávy*/
SELECT u.user_id, u.username, u.email, ctx.ticket_id, ctx.avg_message_length
FROM (
    SELECT t.ticket_id, t.author, AVG(CAST(LEN(m.message) AS FLOAT)) AS avg_message_length
    FROM tickets t
    JOIN messages m ON m.ticket_id = t.ticket_id
    GROUP BY t.ticket_id, t.author
) AS ctx
JOIN users u ON u.user_id = ctx.author
WHERE ctx.avg_message_length >= ALL (
    SELECT AVG(CAST(LEN(m.message) AS FLOAT)) AS avg_message_length
    FROM tickets t
    JOIN messages m ON t.ticket_id = m.ticket_id
    WHERE t.author = ctx.author
    GROUP BY m.ticket_id
)

/*6;2;6;Vypište tickety, jejíž poslední změna kategorie byla odlišný rok a seřaďte podle nejnovější změny*/
SELECT t.ticket_id, t.name, tc.category_id,
       t.created_at, tc.updated_at,
       YEAR(t.created_at) AS ticket_create_year,
       YEAR(tc.updated_at) AS last_category_change_year
FROM (
    SELECT tc.ticket_id, tc.category_id, tc.updated_at
    FROM ticket_categories tc
    WHERE tc.updated_at >= ALL (
        SELECT tc2.updated_at
        FROM ticket_categories tc2
        WHERE tc2.ticket_id = tc.ticket_id
    )
) AS tc
JOIN tickets t ON t.ticket_id = tc.ticket_id
WHERE YEAR(t.created_at) != YEAR(tc.updated_at)
ORDER BY tc.updated_at