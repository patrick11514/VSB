--Vypište počty filmů pro jednotlivé klasifikace (atribut rating)
SELECT f.rating, COUNT(f.film_id) pocet
FROM film f
GROUP BY f.rating
ORDER BY pocet;

--Pro každé id zákazníka vypište počet jeho příjmení. Je ve výsledku něco
--překlavipého?
SELECT c.last_name, COUNT(c.customer_id) pocet
FROM customer c
GROUP BY c.last_name;

--Vypište ID zákazníků, setřízené podle součtu všech jejich plateb
--Zákazníky, kteří provedli žádnou platbu neuvažujte
SELECT p.customer_id
FROM customer c
    JOIN payment p ON c.customer_id = p.customer_id
GROUP BY p.customer_id
ORDER BY SUM(p.amount);

--Pro každé jméno a příjmení herce vypište počet herců s takovým jménem
-- a příjmením. Výsledky setřiďte dle počtu sestupně
SELECT a.first_name, a.last_name, COUNT(*) pocet
FROM actor a
GROUP BY a.first_name, a.last_name
ORDER BY pocet DESC;

--Vypište součty všech plateb za jednotlivé roky a měsíce. Výsledek uspořádejte
--podle roků a měsíců
--Misto DATEPART lze použít YEAR()/MONTH()
SELECT SUM(p.amount) castka, DATEPART(YEAR, p.payment_date) year, DATEPART(MONTH, p.payment_date) month
FROM payment p
GROUP BY DATEPART(YEAR, p.payment_date), DATEPART(MONTH, p.payment_date)
ORDER BY year, month;

--Vypište ID skladu s více než 2300 kopiemi filmů
SELECT i.store_id
FROM inventory i
GROUP BY i.store_id
HAVING COUNT(i.film_id) > 2300;

--Vypište ID jazyků, pro které je nejkratší film delší než 46 minut
SELECT l.language_id
FROM language l
    JOIN film f ON l.language_id = f.language_id
GROUP BY l.language_id
HAVING MIN(f.length) > 46;
--nebo
SELECT f.language_id
FROM film f
GROUP BY f.language_id
HAVING MIN(f.length) > 46;

--Vypište roky a měsíce plateb, kdy byl součet plateb větší než 20 000
SELECT DATEPART(year, p.payment_date) rok, DATEPART(month, p.payment_date) mesic
FROM payment p
GROUP BY DATEPART(year, p.payment_date), DATEPART(month, p.payment_date)
HAVING SUM(p.amount) > 20000;

--Vypište klasifikaci filmů (rating), jejíchž délka je menší než 50 minut a celková
--délka dané klasifikace je větší než 250minut. Výsledky setřiďte podle abecedy
SELECT f.rating
FROM film f
WHERE f.length < 50
GROUP BY f.rating
HAVING SUM(f.length) > 250
ORDER BY f.rating desc;

--Vypište pro jednotlivá ID jazyků počet filmů. Vynechejte jazyky, které nemají žádný
--film.
SELECT f.language_id, COUNT(*) pocet_filmu
FROM film f
GROUP BY f.language_id
HAVING COUNT(*) > 0;

--Vypište názvy jazyků a knim počty filmů. Vynechejte jazyky, které nemají žádný film
SELECT l.name, COUNT(*) pocet_filmu
FROM language l
    JOIN film f ON l.language_id = f.language_id
GROUP BY l.language_id, l.name;

--Vypište pro jednotlivé názvy jazyků počet filmů. Zahrněte i jazyky, které nemají žádný film
SELECT l.name, COUNT(f.film_id) pocet_filmu
FROM language l
    LEFT JOIN film f ON l.language_id = f.language_id
GROUP BY l.language_id, l.name;

--Vypište pro jednotlivé zákazníky (jejich ID, jméno, příjmení) počty jejich výpůjček
SELECT c.customer_id, c.first_name, c.last_name, COUNT(r.rental_id) pocet_vypujcek
FROM customer c
LEFT JOIN rental r ON c.customer_id = r.customer_id
GROUP BY c.customer_id, c.first_name, c.last_name;

--Vypište jednotlivé zákazníky (jejich ID, jméno, příjmení) počty různých filmů, které si vypůjčili

