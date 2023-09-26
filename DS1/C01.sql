--select všech filmů
SELECT * FROM film;

--pouze title
SELECT title FROM film;

SELECT * FROM customer;

--vypíše info o tabulce
sp_help customer;

--Vypište e-mailové adresy všech neaktivních zákazníků.
SELECT email FROM customer WHERE active = 0;

SELECT last_name FROM customer WHERE active = 1;

--Vypište názvy a popisy všech filmů s klasifikací (atribut rating) G. Výstup bude setřízen sestupně podle názvu filmu.
SELECT title, description FROM film WHERE rating = 'G' ORDER BY title DESC;

--Vypište všechny údaje o platbách, které proběhly v roce 2006 nebo později a částka byla menší než 2.
SELECT * FROM payment WHERE payment_date >= '2006-01-01' AND amount < 2;

--Vypište popisy všech filmů klasifikovaných jako G nebo PG
SELECT description FROM film WHERE rating = 'G' OR rating = 'PG';
SELECT description FROM film WHERE rating IN ('G', 'PG');

--Vypište popisy všech filmů klasifikovaných jako G, PG, nebo PG-13
SELECT description FROM film WHERE rating = 'G' OR rating = 'PG' or rating = 'PG-13';
SELECT description FROM film WHERE rating IN ('G', 'PG', 'PG-13');

--Vypište popisy všech filmů, které nejsou klasifikovány jako G, PG, nebo PG-13
SELECT description FROM film WHERE rating NOT IN ('G', 'PG', 'PG-13');
SELECT description FROM film WHERE rating != 'G' AND rating != 'PG' AND rating != 'PG-13';

--Vypište všechny údaje filmů, jejichž délka přesahuje 50 minut a doba výpujčky je 3 nebo 5 dní.
SELECT * FROM film WHERE length > 50 AND (rental_duration = 3 OR rental_duration = 5);
SELECT * FROM film WHERE length > 50 AND rental_duration in (3,5);

--Vypište názvy filmů, které obsahují ”RAINBOW“ nebo začínají na ”TEXAS“ a jejich délka
--přesahuje 70 minut. Zamyslete se nad nejednoznačností formulace této úlohy v přirozeném
--jazyce.
SELECT title FROM film WHERE (title LIKE '%RAINBOW%' or title LIKE 'TEXAS%') and length > 70;

--Vypište názvy všech filmů, v jejichž popisu se vyskytuje ”And“, jejich délka spadá do
--intervalu 80 až 90 minut a standardní doba výpůjčky (atribut rental duration) je liché
--číslo.
SELECT title FROM film WHERE description LIKE '%And%' AND (length >= 80 AND length <= 90) AND rental_duration % 2 != 0;
SELECT title FROM film WHERE description LIKE '%And%' AND length BETWEEN 80 AND 90 AND rental_duration % 2 != 0;

--Vypište vlastnosti (atribut special features) všech filmů, kde částka za náhradu škody
--(atribut replacement cost) je v intervalu 14 až 16. Zajistěte, aby se vlastnosti ve výpisu
--neopakovaly. Setřiďte vybrané vlastnosti abecedně. Zamyslete se, proč je výsledek i bez
--explicitního požadavku na setřízení již abecedně setřízený.
SELECT special_features FROM film where replacement_cost >= 14 and replacement_cost <= 16 GROUP BY special_features;

--Vypište všechny údaje filmů, jejichž standardní doba výpůjčky je menší než 4 dny, nebo
--jsou klasifikovány jako PG. Nesmí však splňovat obě podmínky zároveň.
SELECT * FROM film WHERE (rental_duration < 4 OR rating = 'PG') AND NOT (rental_duration < 4 AND rating = 'PG');

--Vypište všechnyj́udaje o adresách, které mají vyplněno PSČ
SELECT * FROM address WHERE postal_code IS NOT NULL;

--Vypište ID všech zákazníků, kteří aktuálně mají vypůjčený nějaký film.
SELECT distinct customer_id FROM rental WHERE rental.return_date IS NULL;
--Dokážete spočítat, kolik je takových zákazníků?
SELECT COUNT(distinct customer_id) FROM rental WHERE rental.return_date IS NULL;

--Pro každé ID platby vypište v samostatných sloupcích rok, měsíc a den, kdy platba proběhla.
--Sloupce vhodně pojmenujte
SELECT payment_id, DAY(payment_date) Den, MONTH(payment_date) Mesic, YEAR(payment_date) Rok FROM payment;

--Vypište filmy, jejíchž délka názvu není 20 znaků
SELECT * FROM film WHERE len(title) != 20;

--Pro každou ukončenou výpůjčku (její ID) vypište domu jejího trvání v minutách
SELECT rental_id, DATEDIFF(minute, rental_date, return_date) FROM rental WHERE return_date IS NOT NULL;

--Pro každého aktivního zákazníka vypište jeho celé jméno v jednom sloupci.
--Výstup tedy bude obsahovat dva sloupce, customer_id a full_name
SELECT customer_id, CONCAT(first_name, ' ', last_name) full_name FROM customer WHERE active = 1;

--Pro každou adresu (atribut address) vypište PSČ. Jestliže PSČ nebude vyplněno, bude místo něj zobrazovat text "(prázdné)"
--COALESCE - Vrátí první NENULL položku ze seznamu, tedy pokud je postal_code null, tak to tam hodí "(prázdné)"
SELECT address, COALESCE(postal_code, '(prazdne)') PSC FROM address;

--Pro všechny uzavřené půjčky vypište v jednom sloupci interval od - do (t.obě data oddělená pomlčkou), kdy výpůjčka probíhala
SELECT rental_id, CONCAT(rental_date, ' - ' , return_date) interval FROM rental WHERE return_date IS NOT NULL;
SELECT rental_id, CAST(rental_date AS VARCHAR) + ' - ' + CAST(return_date AS VARCHAR) interval FROM rental WHERE return_date IS NOT NULL;

--Pro všechny výpůjčky vypište v jednom sloupci interval od - do (tj. obě data oddělená pomlčkou), kdy výpůjčka probíhala.
--Pokud výpůjčka dostud nebyla vrácena, vypište pouze datum od
SELECT rental_id, CAST(rental_date AS VARCHAR) + COALESCE(CONCAT(' - ', return_date), '') interval FROM rental;
SELECT rental_id, CAST(rental_date AS VARCHAR) + COALESCE(' - ' + CAST(return_date AS VARCHAR), '') interval FROM rental;

--Vypište počet filmů v databází
SELECT COUNT(*) pocet_filmu FROM film;

--Vypište počet různých klasifikací filmů (atribut rating)
SELECT COUNT(distinct rating) pocet_hodnoceni FROM film;

--Vypište jedním dotazem počet adres, počet adres s vyplněným PSČ a počet různých PSČ
SELECT COUNT(*) pocet_adres,  COUNT(postal_code) pocet_adres_s_psc, COUNT(distinct postal_code) ruzne_psc FROM address;

--Vypište nejmenší, největší a průměrnou délku všech filmů. Ověřte zjíštěnou průměrnou pomocí podílu
--součtu a počtu
---CAST AS FLOAT => pokud chci aby výsledek vyšel taky ve floatu
SELECT MIN(length) nejkrasi, MAX(length) nejdelsi, AVG(CAST(length AS FLOAT)) prumerna FROM film;

--Vypište počet a součet všech plateb, které byly provedeny v roce 2005
SELECT COUNT(*) pocet, SUM(amount) soucet FROM payment WHERE YEAR(payment_date) = '2005';

--Vypište celkový počet znaků v názvech všech filmů
SELECT SUM(LEN(title)) pocet_znaku_vsech_filmu FROM film;