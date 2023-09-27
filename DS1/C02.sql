--Vypište všechny informace o městech včetně odpovídajících informací o státech
--kde se tato města nachází
SELECT *
FROM city
    JOIN country ON city.country_id = country.country_id;

--Vypište názvy filmů, včetně jejich jazyka
SELECT film.title, language.name
FROM film
    JOIN language ON film.language_id = language.language_id;

--Vypište ID všech výpůjček zákazníka s příjmením SIMPSON
SELECT r.rental_id
FROM rental r
    JOIN customer c ON r.customer_id = c.customer_id
WHERE c.last_name = 'SIMPSON';

--Vypište adresu (atribut address v tabulce address) zákazníka s příjmením SIMPSON
SELECT a.address
FROM customer c
    JOIN address a ON c.address_id = a.address_id
WHERE c.last_name = 'SIMPSON';

--Pro každého zákazníka (jeho jméno a příjmení) vypište adresu, bydliště včetně názvu města
SELECT c.first_name, c.last_name , a.address, m.city
FROM customer c
    JOIN address a ON c.address_id = a.address_id
    JOIN city m ON a.city_id = m.city_id;

--Pro každého zákazníka (jeho jméno a příjmení) vypište název města, kde bydlí
SELECT c.first_name, c.last_name, m.city
FROM customer c
    JOIN address a ON c.address_id = a.address_id
    JOIN city m ON a.city_id = m.city_id;

--Vypište ID všech výpůjček včetně jména zamněstnance, jména zákazníka a jména filmu
SELECT r.rental_id ,
       s.first_name staff_first_name   ,
       s.last_name  staff_last_name    ,
       c.first_name customer_first_name,
       c.last_name  customer_last_name ,
       f.title
FROM rental r
    JOIN staff s ON r.staff_id = s.staff_id
    JOIN customer c ON r.customer_id = c.customer_id
    JOIN inventory i ON r.inventory_id = i.inventory_id
    JOIN film f ON i.film_id = f.film_id;

--Pro každý film (jeho název) vypište jména a příjmení všech herců, kteří ve filmu hrají.
--Kolik řádků bude ve výsledku tohoto dotahu?
SELECT f.title, a.first_name, a.last_name
FROM film f
    JOIN film_actor fa ON f.film_id = fa.film_id
    JOIN actor a ON fa.actor_id = a.actor_id
ORDER BY f.title;

--Pro každého herce (jeho jméno a příjmení) vypište jména filmů, kde herec hrál.
--Včem je tento dotaz jíný? Nemusíme zde dávat Order BY, protože je již
--seřazen podle herce
SELECT
    a.first_name,
    a.last_name,
    f.title
FROM actor a
    JOIN film_actor fa ON a.actor_id = fa.actor_id
    JOIN film f ON fa.film_id = f.film_id;

--Vypište všechny filmy v kategorii horoor
SELECT f.title
FROM category c
    JOIN film_category fc ON c.category_id = fc.category_id
    JOIN film f ON fc.film_id = f.film_id
WHERE c.name = 'Horror';

--Pro každý sklad (jeho ID) vypište jméno a příjmení jeho správce. Dále vypište adresu
--skladu a adresu správce (u obou adres stačí atribut address v tabulce address)
SELECT st.first_name,
       st.last_name,
       sa.address store_address,
       sta.address staff_address
FROM store s
    JOIN staff st ON s.manager_staff_id = st.staff_id
    JOIN address sa ON s.address_id = sa.address_id
    JOIN address sta ON st.address_id = sta.address_id;
--Řešení dále rozšiřte o adresu včetně města a státu
SELECT st.first_name,
       st.last_name,
       sa.address store_address,
       sac.city store_city,
       saco.country_id store_country,
       sta.address staff_address,
       stac.city staff_city,
       staco.country_id staff_country
FROM store s
    JOIN staff st ON s.manager_staff_id = st.staff_id
    JOIN address sa ON s.address_id = sa.address_id
    JOIN city sac ON sa.city_id = sac.city_id
    JOIN country saco ON sac.country_id = saco.country_id
    JOIN address sta ON st.address_id = sta.address_id
    JOIN city stac ON sta.city_id = stac.city_id
    JOIN country staco ON stac.country_id = staco.country_id;

--Pro každý film (ID a jeho název) vypište ID všech herců a ID všech kategorií, do kterých
--film spadá. Tzn. vypište dotaz, jehož výsledkem bude tabulka s atributy film_id, actor_id
--a category_id, seřazeno podle film_id
SELECT f.film_id, a.actor_id, c.category_id
FROM film f
    JOIN film_actor fa ON f.film_id = fa.film_id
    JOIN actor a ON fa.actor_id = a.actor_id
    JOIN film_category fc ON f.film_id = fc.film_id
    JOIN category c ON fc.category_id = c.category_id
ORDER BY f.film_id;
--Z výsledků pohledem zjístěte, kolik herců hraje ve filmu s filmem id = 1 => 10
SELECT COUNT(distinct a.actor_id)
FROM film f
    JOIN film_actor fa ON f.film_id = fa.film_id
    JOIN actor a ON fa.actor_id = a.actor_id
    JOIN film_category fc ON f.film_id = fc.film_id
    JOIN category c ON fc.category_id = c.category_id
WHERE f.film_id = 1;
--kolik tomuto filmu odpovídá kategorií => 6
SELECT COUNT(distinct c.category_id)
FROM film f
    JOIN film_actor fa ON f.film_id = fa.film_id
    JOIN actor a ON fa.actor_id = a.actor_id
    JOIN film_category fc ON f.film_id = fc.film_id
    JOIN category c ON fc.category_id = c.category_id
WHERE f.film_id = 1;
--kolik tento film v řádků ve výsledu => 60
SELECT COUNT(*)
FROM film f
    JOIN film_actor fa ON f.film_id = fa.film_id
    JOIN actor a ON fa.actor_id = a.actor_id
    JOIN film_category fc ON f.film_id = fc.film_id
    JOIN category c ON fc.category_id = c.category_id
WHERE f.film_id = 1;









--Vypište ID a částku všech plateb a u každé platby uveďte datum výpůjčky, tj.
--hodnotu rental_date v tabulce rental. U plateb, které se nevztahují k žádné
--výpůjčce bude datum výpůjčky NULL
SELECT p.payment_id, p.amount, r.rental_date
FROM payment p
    LEFT JOIN rental r ON p.rental_id = r.rental_id;

