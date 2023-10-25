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