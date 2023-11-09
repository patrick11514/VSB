WITH hlasy AS (
    SELECT *
    FROM zmatecne_hlasovani_poslanec z
    JOIN osoba o ON o.id_osoba = z.id_osoba
    JOIN hlasovani h ON z.id_hlasovani = h.id_hlasovani
)
SELECT h.id_osoba, h.jmeno, h.prijmeni, h.id_organ, COUNT(distinct h.id_hlasovani)
FROM hlasy h
GROUP BY h.id_hlasovani, h.id_organ, h.id_osoba, h.jmeno, h.prijmeni

  SELECT z.id_hlasovani, COUNT(*) --o.id_osoba, o.jmeno, o.prijmeni, h.id_organ, h.id_hlasovani
    FROM zmatecne_hlasovani_poslanec z
    JOIN osoba o ON o.id_osoba = z.id_osoba
    JOIN hlasovani h ON z.id_hlasovani = h.id_hlasovani
    WHERE o.id_osoba = 4
    GROUP BY z.id_hlasovani
    --GROUP BY h.id_hlasovani, h.id_organ, o.id_osoba, o.jmeno, o.prijmeni
    --ORDER BY o.id_osoba

SELECT o.id_osoba, o.jmeno, o.prijmeni, h.id_organ, COUNT(distinct z.id_hlasovani)
    FROM zmatecne_hlasovani_poslanec z
    JOIN osoba o ON o.id_osoba = z.id_osoba
    JOIN hlasovani h ON z.id_hlasovani = h.id_hlasovani
    GROUP BY h.id_hlasovani, h.id_organ, o.id_osoba, o.jmeno, o.prijmeni




sp_help

--ut1230.2
WITH ctx AS (
        SELECT h.id_hlasovani, h.datum, h.nazev_dlouhy, o.zkratka
        FROM hlasovani h
        JOIN hlasovani_poslanec hp ON h.id_hlasovani = hp.id_hlasovani
        JOIN poslanec p ON hp.id_poslanec = p.id_poslanec
        JOIN organ o ON p.id_organ = o.rodic_id_organ
        WHERE h.datum = '2019-10-24' AND
              h.cislo = 172 AND (
                o.zkratka = 'ANO' OR
                o.zkratka = 'ODS'
              )
)
SELECT distinct t.id_hlasovani, t.datum, t.nazev_dlouhy, (
    SELECT COUNT(*)
    FROM ctx t2
    WHERE t2.zkratka = 'ODS'
    ) AS 'ODS',
    (
    SELECT COUNT(*)
    FROM ctx t2
    WHERE t2.zkratka = 'ANO'
    ) AS 'ANO'
FROM ctx t

select id_hlasovani, datum, nazev_dlouhy, (
	SELECT count(*)
	FROM poslanec p
	JOIN zarazeni z ON p.id_osoba = z.id_osoba and z.cl_funkce = 0
	JOIN organ org ON z.id_of = org.id_organ
	JOIN organ psp2013 ON psp2013.id_organ = p.id_organ and org.rodic_id_organ = psp2013.id_organ
	JOIN hlasovani_poslanec hp ON hp.id_poslanec = p.id_poslanec
	WHERE org.zkratka = 'ANO' and
		  psp2013.nazev_organu_cz='Poslanecká sněmovna' and
			YEAR(psp2013.od_organ) = 2017 and
			hh.id_hlasovani = hp.id_hlasovani and
			hp.vysledek = 'A'
) Ano_pro, (
	SELECT count(*)
	FROM poslanec p
	JOIN zarazeni z ON p.id_osoba = z.id_osoba and z.cl_funkce = 0
	JOIN organ org ON z.id_of = org.id_organ
	JOIN organ psp2013 ON psp2013.id_organ = p.id_organ and org.rodic_id_organ = psp2013.id_organ
	JOIN hlasovani_poslanec hp ON hp.id_poslanec = p.id_poslanec
	WHERE org.zkratka = 'ODS' and
		  psp2013.nazev_organu_cz='Poslanecká sněmovna' and
			YEAR(psp2013.od_organ) = 2017 and
			hh.id_hlasovani = hp.id_hlasovani and
			hp.vysledek = 'A'
) ODS_pro
from hlasovani hh
where hh.datum = '2019-10-24' and cislo = 172


----Korektní propojení poslance a organ ve kterem je
SELECT *
	FROM poslanec p
	JOIN zarazeni z ON p.id_osoba = z.id_osoba and z.cl_funkce = 0
	JOIN organ org ON z.id_of = org.id_organ
	JOIN organ psp2013 ON psp2013.id_organ = p.id_organ and org.rodic_id_organ = psp2013.id_organ


--ut 14:15 A 1
--OK
--Nalezněte poslanecké kluby, které měly dvě a více volebních období v poslanecké sněmovně mi-
--nimálně 50 poslanců (stačí počet záznamů v tabulce poslanec pro dané období). Vypište zkratku
--poslaneckého klubu a počet volebních období, kdy měl poslanecký klub více než 50 poslanců.
--Poslanecký klub je záznam v organ pro který typ organu.nazev typ org cz = ’klub’.

WITH ctx AS (
    SELECT org.zkratka
    FROM poslanec p
    JOIN zarazeni z ON p.id_osoba = z.id_osoba
    JOIN organ org ON org.id_organ = z.id_of AND cl_funkce = 0
    JOIN organ org2 ON org2.id_organ = p.id_organ AND org.rodic_id_organ = org2.id_organ
    JOIN typ_organu torg ON torg.id_typ_org = org.id_typ_org
    WHERE torg.nazev_typ_org_cz = 'klub'
    GROUP BY org.rodic_id_organ, org.zkratka
    HAVING COUNT(*) >= 50
)
SELECT zkratka, COUNT(*)
FROM ctx
GROUP BY zkratka
HAVING COUNT(*) >= 2

SELECT *
FROM organ org
JOIN organ org2 ON org.rodic_id_organ = org2.id_organ
WHERE YEAR(org.od_organ) = 2013

--ut 14:15 A 2
--OK
--Pro každou schůzi ve volebním období 2013 - 2017 nalezněte poslední hlasování. K porovnání
--pořadí můžete využít atribut hlasovani.cislo. Tzn. zajímá nás hlasování s nejvyšší hodnotou
--atributu hlasovani.cislo v rámci jedné schůze. Setřiďte výsledek podle atributu schuze.schuze
--a vypište atributy id hlasovani, schuze, cislo a schuze.nazev dlouhy.


WITH ctx AS (
SELECT h.id_hlasovani, s.schuze, h.cislo, h.nazev_dlouhy
FROM schuze s
JOIN hlasovani h ON h.schuze = s.schuze AND s.id_organ = h.id_organ --POZOR, musim joinout i
JOIN organ org ON org.id_organ = s.id_organ
WHERE YEAR(org.od_organ) = 2013
)
SELECT id_hlasovani, schuze, cislo, nazev_dlouhy
FROM ctx t1
WHERE cislo >= ALL(
    SELECT cislo
    FROM ctx t2
    WHERE t1.schuze = t2.schuze
    )
ORDER BY t1.schuze

-- ut 14:15 A 3
--??
SELECT *
FROM poslanec p
JOIN osoba o ON p.id_osoba = o.id_osoba
JOIN zarazeni z ON z.id_osoba = o.id_osoba
JOIN organ org ON org.id_organ = z.id_of AND z.cl_funkce = 0
JOIN organ org2 ON org2.rodic_id_organ = org.id_organ
WHERE YEAR(org.od_organ) = 2017


-- ut 14:15 B 1
-- OK
--Nalezněte volební období, ve kterém byly v poslanecké sněmovně alespoň dva kluby s minimálně
--50 poslanci (stačí počet záznamů v tabulce poslanec pro dané období). Vypište id volebního ob-
--dobí (id organ), kdy volební období začínalo (od organ) a počet klubů s více než 50 poslanci.
--Poslanecký klub je záznam v organ pro který typ organu.nazev typ org cz = ’klub’.

WITH ctx AS (SELECT org2.id_organ, org.zkratka, org2.od_organ
             FROM poslanec p
                      JOIN zarazeni z ON z.id_osoba = p.id_osoba
                      JOIN organ org ON org.id_organ = z.id_of AND z.cl_funkce = 0
                      JOIN organ org2 ON org2.id_organ = p.id_organ AND org.rodic_id_organ = org2.id_organ
                      JOIN typ_organu torg ON torg.id_typ_org = org.id_typ_org
             WHERE torg.nazev_typ_org_cz = 'klub'
             GROUP BY org2.id_organ, org.zkratka, org2.od_organ
             HAVING COUNT(*) >= 50)
SELECT t1.id_organ, t1.od_organ
FROM ctx t1
GROUP BY t1.id_organ, t1.od_organ
HAVING COUNT(*) >= 2


-- ut 14:15 B 2
--OK
--Pro každou schůzi ve volebním období 2002 - 2006 nalezněte poslední hlasování. K porovnání
--pořadí můžete využít atribut hlasovani.cislo. Tzn. zajímá nás hlasování s nejvyšší hodnotou
--atributu hlasovani.cislo v rámci jedné schůze. Setřiďte výsledek podle atributu schuze.schuze
--a vypište atributy id hlasovani, schuze, cislo a schuze.nazev dlouhy.

WITH ctx AS (SELECT h.id_hlasovani, s.schuze, h.cislo, h.nazev_dlouhy
             FROM schuze s
                      JOIN organ org ON s.id_organ = org.id_organ
                      JOIN hlasovani h ON s.schuze = h.schuze AND h.id_organ = s.id_organ
            WHERE YEAR(org.od_organ) = 2002
)
SELECT t1.id_hlasovani, t1.schuze, t1.cislo, t1.nazev_dlouhy
FROM ctx t1
WHERE t1.cislo >= ALL(
    SELECT t2.cislo
    FROM ctx t2
    WHERE t1.schuze = t2.schuze
    )

-- ut 14:15 B 3
-- OK ?? XD
--Nalezněte osoby, které nikdy ve volebním období 2017 - 2021 nehlasovaly PRO u hlasování, jehož
--dlouhý název (t.j. hlasovani.nazev dlouhy) obsahuje ”COVID”. Vyberte pouze osoby, které
--hlasovaly v nějakém takovém hlasování, ale nikdy nehlasovali pro. Vypište všechny údaje osoby a
--setřiďte je podle id osoba. Hlasování PRO se pozná tak, že v hlasovani poslanec.vysledek
--= ’A’.

SELECT *
FROM poslanec p
JOIN osoba os ON os.id_osoba = p.id_osoba
JOIN zarazeni z ON os.id_osoba = z.id_osoba
JOIN organ org ON org.id_organ = z.id_of AND z.cl_funkce = 0
JOIN organ org2 ON org2.id_organ = p.id_organ AND org.rodic_id_organ = org2.id_organ
JOIN hlasovani_poslanec hp ON hp.id_poslanec = p.id_poslanec
JOIN hlasovani h ON hp.id_hlasovani = h.id_hlasovani
WHERE YEAR(org.od_organ) = 2017

SELECT *
FROM osoba os
WHERE NOT EXISTS (
    SELECT 1
    FROM hlasovani h
    JOIN hlasovani_poslanec hp ON hp.id_hlasovani = h.id_hlasovani
    JOIN organ org ON h.id_organ = org.id_organ
    JOIN poslanec p ON p.id_poslanec = hp.id_poslanec
    WHERE YEAR(org.od_organ) = 2017 AND
          hp.vysledek = 'A' AND
          h.nazev_dlouhy LIKE '%COVID%' AND
          p.id_osoba = os.id_osoba
    ) AND EXISTS (
        SELECT 1
    FROM hlasovani h
    JOIN hlasovani_poslanec hp ON hp.id_hlasovani = h.id_hlasovani
    JOIN organ org ON h.id_organ = org.id_organ
    JOIN poslanec p ON p.id_poslanec = hp.id_poslanec
    WHERE YEAR(org.od_organ) = 2017 AND
          h.nazev_dlouhy LIKE '%COVID%' AND
          p.id_osoba = os.id_osoba
)

-- st 9:00 1 A
-- ?? XD


-- st 9:00 2 A
-- ok
--Vypište osoby (id osoba, jmeno a příjmení), které se častěji omlouvaly v lednu než v únoru a
--a zároveň počet omluv v lednu > 33. Setřiďte sestupně dle počtu omluv v lednu.

WITH ctx AS (SELECT os.id_osoba,
                    os.jmeno,
                    os.prijmeni,
                    COUNT(CASE WHEN MONTH(o.den) = 1 then 1 end) AS pocet_omluv_leden, -- TAKHLE FUNGUJE CASE BTW :)) jinak bych měl na testu nějaký bodíky :/
                    COUNT(CASE WHEN MONTH(o.den) = 2 then 1 end) AS pocet_omluv_unor
             FROM omluva o
                      JOIN poslanec p ON o.id_poslanec = p.id_poslanec
                      JOIN osoba os ON os.id_osoba = p.id_osoba
             GROUP BY os.id_osoba, os.jmeno, os.prijmeni)
SELECT *
FROM ctx
WHERE ctx.pocet_omluv_leden > 33 AND
      ctx.pocet_omluv_leden > ctx.pocet_omluv_unor
ORDER BY ctx.pocet_omluv_leden DESC


--st 9:00 3 A
--OK
--Vypište osoby, které byly místopředsedou sněmovny (typ funkce.typ funkce cz = ’Místopředseda’,
--typ organu.nazev typ org cz = ’Parlament’) více než jednou. Pro každou osobu vypište: id osoba,
--jméno, příjmení, počet těchto funkcí, počet členství v delegacích (typ organu.nazev typ org cz
--= ’Delegace’). Setřiďte sestupně dle počtu místopřesednické funkce.

SELECT os.id_osoba, os.jmeno, os.prijmeni, COUNT(*) AS pocet, (
        SELECT COUNT(*)
        FROM osoba o2
        JOIN zarazeni z ON z.id_osoba = os.id_osoba
        JOIN organ org ON org.id_organ = z.id_of AND z.cl_funkce = 0
        JOIN typ_organu torg ON torg.id_typ_org = org.id_typ_org
        WHERE o2.id_osoba = os.id_osoba AND
              torg.nazev_typ_org_cz = 'Delegace'
    ) AS pocet_clenstvi_delegace
FROM osoba os
JOIN zarazeni z ON z.id_osoba = os.id_osoba
JOIN funkce f ON f.id_funkce = z.id_of AND cl_funkce = 1
JOIN typ_funkce tf ON tf.id_typ_funkce = f.id_typ_funkce
JOIN typ_organu torg ON torg.id_typ_org = tf.id_typ_org
WHERE tf.typ_funkce_cz = 'Místopředseda' AND
      torg.nazev_typ_org_cz = 'Parlament'
GROUP BY os.id_osoba, os.jmeno, os.prijmeni
HAVING COUNT(*) > 1
ORDER BY pocet DESC

--st 9:00 1 B
--OK
--Vypište osoby, které byly v nějakém výboru poslanecké sněmovny (typ organ.nazev typ org cz
--= ’Výbor’) a zároveň ve stejném volebním období (tj. orgány mají stejnou hodnotu organ.
--rodic id organ) nebyly v žádné delegaci poslanecké sněmovny (typ organ.nazev typ org cz
--= ’Delegace’). Pro každou osobu vypište: id osoba, jméno a příjmení osoby, počet takových členství
--ve výborech, počet všech členství v delegacích (pokud nebyl poslanec členem žádné delegace,
--vypište hodnotu 0).
--Setřiďte dle počtu členství ve všech delegacích. Uvažujte pouze osoby, které byly členy takových
--výborů 8x.

SELECT os.id_osoba, os.jmeno, os.prijmeni, COUNT(*) AS pocet_vybor, (
    SELECT COUNT(*)
    FROM osoba os2
    JOIN zarazeni z2 ON z2.id_osoba = os2.id_osoba
    JOIN organ org2 ON org2.id_organ =  z2.id_of AND z2.cl_funkce = 0
    JOIN typ_organu torg2 ON torg2.id_typ_org = org2.id_typ_org
    WHERE torg2.nazev_typ_org_cz = 'Delegace' AND
          os2.id_osoba = os.id_osoba
    ) AS pocet_delegace
FROM osoba os
JOIN zarazeni z ON z.id_osoba = os.id_osoba
JOIN organ org ON org.id_organ =  z.id_of AND z.cl_funkce = 0
JOIN typ_organu torg ON torg.id_typ_org = org.id_typ_org
WHERE torg.nazev_typ_org_cz = 'Výbor' AND
      NOT EXISTS(
          SELECT 1
          FROM osoba os2
          JOIN zarazeni z2 ON z2.id_osoba = os2.id_osoba
          JOIN organ org2 ON org2.id_organ =  z2.id_of AND z2.cl_funkce = 0
          JOIN typ_organu torg2 ON torg2.id_typ_org = org2.id_typ_org
          WHERE os2.id_osoba = os.id_osoba AND
                org2.rodic_id_organ =org.rodic_id_organ AND
                torg2.nazev_typ_org_cz = 'Delegace'

      )
GROUP BY os.id_osoba, os.jmeno, os.prijmeni
HAVING COUNT(*) = 8
ORDER BY pocet_delegace DESC

--st 9:00 2 B
WITH ctx AS (
    SELECT os.id_osoba, os.jmeno, os.prijmeni,
           COUNT(CASE WHEN MONTH(oml.den) = 4 THEN 1 END) AS pocet_dnu_duben,
           COUNT(CASE WHEN MONTH(oml.den) = 5 THEN 1 END) AS pocet_dnu_kveten
    FROM osoba os
    JOIN poslanec pos ON os.id_osoba = pos.id_osoba
    JOIN omluva oml ON oml.id_poslanec = pos.id_poslanec
    JOIN organ org ON org.id_organ = oml.id_organ
    WHERE YEAR(org.od_organ) = 2021
    GROUP BY os.id_osoba, os.jmeno, os.prijmeni
)
SELECT *
FROM ctx
WHERE pocet_dnu_duben > 11 AND
      pocet_dnu_duben > pocet_dnu_kveten
ORDER BY pocet_dnu_duben DESC

