--1
SELECT f.film_id, f.title, (
        SELECT COUNT(*) as pocet
        FROM film_actor fa
        WHERE fa.film_id = f.film_id
    ) hercu,
    (
        SELECT COUNT(*)
        FROM film_category fc
        WHERE f.film_id = fc.film_id
    ) kategorii
FROM film f

--2
SELECT c.customer_id, c.first_name, c.last_name, (
        SELECT COUNT(*)
        FROM rental r
        WHERE r.customer_id = c.customer_id AND
              DATEDIFF(DAY, r.rental_date, r.return_date) < 5
    ) mene_nez_5_dni,
    (
        SELECT COUNT(*)
        FROM rental r
        WHERE r.customer_id = c.customer_id AND
              DATEDIFF(DAY,  r.rental_date, r.return_date) < 7
    ) mene_nez_7_dni
FROM customer c

--3
SELECT s.store_id, (
        SELECT COUNT(*)
        FROM inventory i
        JOIN film f ON f.film_id = i.film_id
        JOIN language l ON f.language_id = l.language_id
        WHERE i.store_id = s.store_id AND
              l.name = 'English'
    ) anglicke,
    (
        SELECT COUNT(*)
        FROM inventory i
        JOIN film f ON f.film_id = i.film_id
        JOIN language l ON f.language_id = l.language_id
        WHERE i.store_id = s.store_id AND
              l.name = 'French'
    ) francouzske
FROM store s

--3.1
WITH t (
    SELECT i.store_id, l.name
    FROM inventory i
    JOIN film f ON f.film_id = i.film_id
    JOIN language l ON f.language_id = l.language_id
) SELECT s.store_id,
       (
           SELECT COUNT(*)
           FROM t
           WHERE t.store_id = s.store_id AND
                 t.name = 'English'
           ) anglicke,
        (
           SELECT COUNT(*)
           FROM t
           WHERE t.store_id = s.store_id AND
                 t.name = 'French'
           ) francouzske
FROM store s
