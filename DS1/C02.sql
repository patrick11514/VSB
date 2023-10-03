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

--Vypište všechny kombinace atributů ID herce a ID kategorie, kde daný herec hrál ve filmu v dané kategorii. Výsledek
-- setřiďte dle ID herce.
SELECT distinct fa.actor_id, fc.category_id
FROM film f
    JOIN film_actor fa ON f.film_id = fa.film_id
    JOIN film_category fc ON f.film_id = fc.film_id
ORDER BY fa.actor_id;
--Dotaz dále rozšiřte o výpis jména a příjmení herce a názvu kategorie
SELECT distinct fa.actor_id, fc.category_id, a.first_name, a.last_name, c.name category_name
FROM film f
    JOIN film_actor fa ON f.film_id = fa.film_id
    JOIN film_category fc ON f.film_id = fc.film_id
    JOIN actor a ON fa.actor_id = a.actor_id
    JOIN category c ON fc.category_id = c.category_id
ORDER BY fa.actor_id;

--Vypište jména filmů, které půjčovna vlastní alespoň v jené kopii
SELECT distinct f.title
FROM inventory i
    JOIN film f ON i.film_id = f.film_id;

--Zjistěte jména herců, kteří hrají v nějaké komedii (kategorie "Comedy")
SELECT distinct a.first_name
FROM film f
    JOIN film_actor fa ON f.film_id = fa.film_id
    JOIN actor a ON fa.actor_id = a.actor_id
    JOIN film_category fc ON f.film_id = fc.film_id
    JOIN category c ON fc.category_id = c.category_id
WHERE c.name = 'Comedy';

--Vypište jména všech zákazníků, kteří pocházení z Itálie a někdy měli, nebo mají
--vypůjčený film s názvem MOTIONS DETAILS
SELECT distinct cu.first_name, cu.last_name
FROM rental r
    JOIN inventory i ON r.inventory_id = i.inventory_id
    JOIN customer cu ON r.customer_id = cu.customer_id
    JOIN address a ON cu.address_id = a.address_id
    JOIN city ci ON a.city_id = ci.city_id
    JOIN country co ON ci.country_id = co.country_id
    JOIN film f on i.film_id = f.film_id
WHERE
    co.country = 'Italy' AND
    f.title = 'MOTIONS DETAILS';

--Zjistěte jména a příjmení všech zákazníků ,kteří mají akzuálně vypůjčený nějaký film
--kde hraje herec SEAN GUINESS
SELECT distinct cu.first_name, cu.last_name
FROM rental r
    JOIN customer cu ON r.customer_id = cu.customer_id
    JOIN inventory i ON r.inventory_id = i.inventory_id
    JOIN film_actor fa ON i.film_id = fa.film_id
    JOIN actor a ON fa.actor_id = a.actor_id
WHERE
    a.first_name = 'SEAN' AND
    a.last_name = 'GUINESS' AND
    r.return_date IS NULL;

--Vypište ID a částku všech plateb a u každé platby uveďte datum výpůjčky, tj.
--hodnotu rental_date v tabulce rental. U plateb, které se nevztahují k žádné
--výpůjčce bude datum výpůjčky NULL
SELECT p.payment_id, p.amount, r.rental_date
FROM payment p
    LEFT JOIN rental r ON p.rental_id = r.rental_id;

--Pro každý jazyk vypište název filmů v daném jazyce. Zajistěte, aby byl jazyk
--ve výsledku obsažen, i když k němui nebude existovat odpovídající film
SELECT l.name, f.title
FROM language l
    LEFT JOIN film f ON l.language_id = f.language_id;

--Pro každý film (ID a název) vypište jeho jazyk a původní jazyk
SELECT f.film_id, f.title, ol.name original_language, l.name language
FROM film f
    LEFT JOIN language ol ON f.original_language_id = ol.language_id
    JOIN language l ON f.language_id = l.language_id;

--Vypište názvy filmů, které si někdy půjčil zákazník TIM CARY, nebo je jejich délka 48 minut
SELECT distinct f.title
FROM film f
    LEFT JOIN inventory i ON f.film_id = i.film_id
    LEFT JOIN rental r ON i.inventory_id = r.inventory_id
    LEFT JOIN customer c ON r.customer_id = c.customer_id
WHERE (
    c.first_name = 'TIM' AND
    c.last_name = 'CARY'
    )
   OR
    f.length =48;

--Vypište názvy filmů, které půjčovna nevlastní ani v jedné kopii (tj. nejsou v inventářiú
SELECT f.title
FROM film f
    LEFT JOIN inventory i ON f.film_id = i.film_id
WHERE i.film_id IS NULL;

--Vypište jména a příjmení všech zákazníků, kteří mají nějakou nezaplacenou výpůjčku
SELECT distinct c.first_name, c.last_name
FROM rental r
    LEFT JOIN payment p ON r.rental_id = p.rental_id
    JOIN customer c ON r.customer_id = c.customer_id
WHERE p.payment_id IS NULL;

--U každého názvu filmu vypište jazyk filmu, pokud jazyk začíná písmenem "I", v opačném případě
--bude jazyk NULL
SELECT f.title, l.name
FROM film f
    LEFT JOIN language l ON
        f.language_id = l.language_id AND
        l.name LIKE 'I%';

--Pro každého zákazníka vypište ID všech plateb s částkou větší než 9. U zákazníků, kteří takvé
--platby nemají, bude payment_id rovno NULL
SELECT c.customer_id, c.first_name, c.last_name, p.payment_id
FROM customer c
    LEFT JOIN payment p ON
        p.customer_id = c.customer_id AND
        p.amount > 9;

--Pro každou výpůjčku (její ID) vypište název filmu, pokud obsahuje písmeno U, a město a stát zákazníka
--jehož adresa obsahuje písmeno A. Pokud nejsou podmínky splněny => null
SELECT r.rental_id, f.title, ci.city, co.country
FROM rental r
    LEFT JOIN customer c ON r.customer_id = c.customer_id
    LEFT JOIN address a ON
        c.address_id = a.address_id AND
        a.address LIKE '%A%'
    LEFT JOIN city ci ON a.city_id = ci.city_id
    LEFT JOIN country co ON ci.country_id = co.country_id
    LEFT JOIN inventory i ON r.inventory_id = i.inventory_id
    LEFT JOIN film f ON
        i.film_id = f.film_id AND
        f.title LIKE '%U%';

--Vypište všechny dvojice název filmu a příjmení zákazníka, kde si zákazník vypůjčil daný film. Pokud
--výpůjčka proběhla po datu 1.1.2006, bude příjmení zákazníka nevyplněné (NULL). Z výsledu odstraňte
--duplicitní řádký a setříďte jej podle názvu filmu
SELECT distinct f.title, c.last_name
FROM rental r
    LEFT JOIN customer c ON
        r.customer_id = c.customer_id AND
        r.rental_date <= '2006-01-01'
    JOIN inventory i ON r.inventory_id = i.inventory_id
    JOIN film f ON i.film_id = f.film_id
ORDER BY f.title;