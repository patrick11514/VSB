--Vypište ID a názevy filmů, ve kterých hrál herec s ID 1, bez použití JOIN
SELECT f.film_id, f.title
FROM film f
WHERE f.film_id
          IN (
              SELECT fa.film_id
              FROM film_actor fa
              WHERE fa.actor_id = 1
          );
SELECT f.film_id, f.title
FROM film f
WHERE EXISTS (
        SELECT 1
        FROM film_actor fa
        WHERE fa.actor_id = 1 AND
              fa.film_id = f.film_id
    );
SELECT f.film_id, f.title
FROM film f
JOIN film_actor fa ON f.film_id = fa.film_id
WHERE fa.actor_id = 1;

--Vypište ID filmů, ve kterých hrál herec s ID = 1
SELECT fa.film_id
FROM film_actor fa
WHERE fa.actor_id = 1;

--Vypište ID a názvy filmů, ve kterých hrál herec s ID = 1 a zároveň herec s ID = 10
SELECT f.film_id
FROM film f
WHERE f.film_id
          IN (
            SELECT fa.film_id
            FROM film_actor fa
            WHERE fa.actor_id = 1
          ) AND
      f.film_id
          IN (
            SELECT fa.film_id
            FROM film_actor fa
            WHERE fa.actor_id = 10
          );

--Vypište ID a názvy filmů ve kterých hrál herec s ID = 1 nebo herec s ID = 10
SELECT f.film_id, f.title
FROM film f
WHERE f.film_id
          IN (
              SELECT fa.film_id
              FROM film_actor fa
              WHERE fa.actor_id = 1 OR
                    fa.actor_id = 10
          );

--Vypište id filmů, ve kterých nehrál herec s ID = 1
SELECT f.film_id
FROM film f
WHERE f.film_id NOT IN (
    SELECT fa.film_id
    FROM film_actor fa
    WHERE fa.actor_id = 1
    );

--Vypišt
SELECT f.film_id, f.title
FROM film f
WHERE f.film_id IN (
        SELECT fa.film_id
        FROM film_actor fa
        WHERE fa.actor_id IN (1,10)
    ) AND NOT (
        f.film_id IN (
            SELECT fa.film_id
            FROM film_actor fa
            WHERE fa.actor_id = 1
        ) AND
        f.film_id IN (
            SELECT fa.film_id
            FROM film_actor fa
            WHERE fa.actor_id = 10
        )
    );

--
SELECT f.film_id, f.title
FROM film f
WHERE f.film_id IN (
        SELECT fa.film_id
        FROM film_actor fa
        JOIN actor a ON a.actor_id = fa.actor_id
        WHERE a.first_name = 'PENELOPE' AND a.last_name = 'GUINESS'
    ) AND
    f.film_id IN (
        SELECT fa.film_id
        FROM film_actor fa
        JOIN actor a ON a.actor_id = fa.actor_id
        WHERE a.first_name = 'CHRISTIAN' AND a.last_name = 'GABLE'
        );

--
SELECT f.film_id, f.title
FROM film f
WHERE f.film_id NOT IN (
        SELECT fa.film_id
        FROM film_actor fa
        JOIN actor a ON a.actor_id = fa.actor_id
        WHERE a.first_name = 'PENELOPE' AND
              a.last_name = 'GUINESS'
    );