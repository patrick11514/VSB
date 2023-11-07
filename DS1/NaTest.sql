SELECT p.*
FROM poslanec p
JOIN organ org ON p.id_organ = org.id_organ
WHERE YEAR(org.od_organ) = 2013

--Všechny orgány přiřazené proslanci jménem Andrej Babiš
SELECT o.*
FROM osoba o
JOIN zarazeni z ON o.id_osoba = z.id_osoba
JOIN organ org ON z.id_of=org.id_organ AND cl_funkce = 0
WHERE o.jmeno = 'Andrej' AND
      o.prijmeni = 'Babis'

-- Všechny funkce přiřazené poslanci jménem Marek Benda
SELECT f.*
FROM osoba o
JOIN zarazeni z ON z.id_osoba = z.od_osoba
JOIN funkce f ON z.id_of=f.id_funkce AND z.cl_funkce = 1
WHERE o.jmeno = 'Marek' AND
      o.prijmeni = 'Benda'

--Všichni předsedové sněmovny, id_organu sněmovny a počáteční datum funkce
SELECT org.id_organ, z.od_o,  o.jmeno, o.prijmeni
FROM osoba o
JOIN zarazeni z ON o.id_osoba = z.id_osoba
JOIN funkce f ON z.id_of=f.id_funkce AND z.cl_funkce = 1
JOIN typ_funkce tf on f.id_typ_funkce = tf.id_typ_funkce
JOIN typ_organu tor ON tf.id_typ_org = tor.id_typ_org
JOIN organ org ON f.id_organ = org.id_organ
WHERE tf.typ_funkce_cz = 'Předseda' AND tor.nazev_typ_org_cz = 'Parlament'
ORDER BY z.od_o

--Všichni místopředsedové aktálního období sněmovny (tj. sněmovny začínající 2021)

SELECT org.id_organ, tf.typ_funkce_cz, z.od_o, z.do_o, o.jmeno, o.prijmeni
FROM osoba o
JOIN zarazeni z ON o.id_osoba = z.id_osoba
JOIN funkce f ON z.id_of = f.id_funkce AND z.cl_funkce = 1
JOIN typ_funkce tf ON f.id_typ_funkce = tf.id_typ_org
JOIN typ_organu tor ON tor.id_typ_org = tf.id_typ_org
JOIN organ org ON f.id_organ =


-- Seznam všech aktuálních poslanců altuální sněmovny
SELECT o.id_osoba, o.jmeno, o.prijmeni
FROM osoba o
JOIN dbo.zarazeni z ON o.id_osoba = z.id_osoba
JOIN organ org ON z.id_of=org.id_organ AND cl_funkce = 0
WHERE org.nazev_organu_cz = 'Poslanecká sněmovna' AND
      year(org.od_organ) = 2021 AND
      z.do_o is NULL



-- REAL PRIPRAVA XDD

SELECT pred, jmeno, prijmeni, DATEDIFF(YEAR, narozeni, CURRENT_TIMESTAMP) as vek
FROM osoba
WHERE DATEDIFF(YEAR, narozeni, CURRENT_TIMESTAMP) < 30

SELECT o.pohlavi, COUNT(*) pocet, (CAST(COUNT(*) AS FLOAT) / (
    SELECT COUNT(*) FROM osoba
    )) * 100 AS pomer
FROM osoba o
GROUP BY o.pohlavi


SELECT id_organ, COUNT(*) pocet
FROM poslanec
GROUP BY id_organ
ORDER BY id_organ


SELECT id_organ
FROM poslanec p
JOIN osoba o ON p.id_osoba = o.id_osoba
WHERE o.jmeno = 'Andrej' AND
      o.prijmeni = 'Babiš'

SELECT o.jmeno, o.prijmeni, p.id_kandidatka
FROM poslanec p
JOIN osoba o ON p.id_osoba = o.id_osoba
WHERE p.id_organ = 173
ORDER BY id_kandidatka

SELECT id_kandidatka, COUNT(*) pocet
FROM poslanec p
WHERE p.id_organ = 173
GROUP BY id_kandidatka

SELECT distinct nazev_typ_org_cz
FROM organ o
JOIN typ_organu tyo ON o.id_typ_org = tyo.id_typ_org
WHERE o.rodic_id_organ = 173

SELECT distinct nazev_organu_cz
FROM organ o
JOIN typ_organu tyo ON o.id_typ_org = tyo.id_typ_org
WHERE rodic_id_organ = 173 AND
      tyo.nazev_typ_org_cz = 'Klub'



--


SELECT distinct s.id_schuze, s.schuze, s.od_schuze
FROM schuze s
JOIN hlasovani h ON s.schuze = h.schuze
LEFT JOIN zmatecne z ON z.id_hlasovani = h.id_hlasovani
WHERE DATEPART(YEAR, s.do_schuze) = 2020 AND
      z.id_hlasovani IS NOT NULL
ORDER BY s.id_schuze

SELECT *
FROM schuze s
JOIN hlasovani h ON s.schuze = h.schuze
LEFT JOIN zmatecne z ON z.id_hlasovani = h.id_hlasovani
WHERE DATEPART(YEAR, s.do_schuze) = 2020 AND
      z.id_hlasovani IS NULL AND
      s.id_schuze = 521


SELECT distinct os.prijmeni, os.jmeno, o.id_organ, COUNT(*) pocet
FROM organ o
JOIN poslanec p ON o.rodic_id_organ = p.id_organ
JOIN osoba os ON p.id_osoba = os.id_osoba
JOIN zmatecne_hlasovani_poslanec zhp ON p.id_osoba = zhp.id_osoba
GROUP BY zhp.id_hlasovani, o.rodic_id_organ, os.prijmeni, os.jmeno, o.id_organ
ORDER BY pocet DESC


