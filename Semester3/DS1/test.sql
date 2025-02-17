--2

WITH ctx as (
    SELECT os.id_osoba, os.jmeno, os.prijmeni, oml.den--, DATEPART(MONTH, oml.den) AS mesic, COUNT(*) pocet
    FROM organ o
    JOIN poslanec p ON p.id_organ = o.rodic_id_organ
    JOIN osoba os ON p.id_osoba = os.id_osoba
    JOIN omluva oml ON oml.id_poslanec = p.id_poslanec
    WHERE o.nazev_organu_cz = 'Poslanecký klub Občanské demokratické strany'
    GROUP BY
)
SELECT ctx.id_osoba,
       ctx.jmeno,
       ctx.prijmeni,
       ctx.pocet AS pocet_omluvy_listopad,
       ctx2.pocet AS pocet_omluvy_prosinec
FROM ctx
JOIN ctx ctx2 ON ctx.id_osoba = ctx2.id_osoba AND
                 ctx2.mesic = 12
WHERE ctx.mesic = 11 AND ctx.pocet > 16 AND ctx.pocet > ctx2.pocet


SELECT os.jmeno, os.prijmeni
FROM organ o
JOIN poslanec p ON p.id_organ = o.rodic_id_organ
JOIN osoba os ON p.id_osoba = os.id_osoba
JOIN omluva oml ON oml.id_poslanec = p.id_poslanec
WHERE o.nazev_organu_cz = 'Poslanecký klub Občanské demokratické strany' AND
      DATEPART(MONTH, oml.den) = 11
GROUP BY os.id_osoba, os.jmeno, os.prijmeni
HAVING COUNT(*) > 16

SELECT COUNT(*)
FROM organ o
JOIN poslanec p ON p.id_organ = o.rodic_id_organ
JOIN osoba os ON p.id_osoba = os.id_osoba
JOIN omluva oml ON oml.id_poslanec = p.id_poslanec
WHERE o.nazev_organu_cz = 'Poslanecký klub Občanské demokratické strany' AND
      DATEPART(MONTH, oml.den) = 12
GROUP BY os.id_osoba


SELECT os.jmeno, os.prijmeni, DATEPART(MONTH, oml.den)
FROM organ o
JOIN poslanec p ON p.id_organ = o.rodic_id_organ
JOIN osoba os ON p.id_osoba = os.id_osoba
JOIN omluva oml ON oml.id_poslanec = p.id_poslanec
WHERE o.nazev_organu_cz = 'Poslanecký klub Občanské demokratické strany'


--3
WITH ctx AS (
    SELECT o.id_osoba,
           o.jmeno,
           o.prijmeni,
           org.nazev_organu_cz,
           COUNT(org.id_organ) AS pocet_volebnich_obdobi,
           COUNT(org.rodic_id_organ) AS pocet_klubu
    FROM osoba o
        JOIN poslanec p ON o.id_osoba = p.id_osoba
        JOIN organ org ON p.id_organ = org.rodic_id_organ
        JOIN typ_organu orgt ON org.id_typ_org = orgt.id_typ_org
    WHERE orgt.nazev_typ_org_cz = 'Klub' AND org.nazev_organu_cz != 'Nezařazení'
    GROUP BY org.nazev_organu_cz, o.id_osoba, o.jmeno, o.prijmeni
    HAVING COUNT(org.id_organ) > 6
)
SELECT c1.id_osoba, c1.jmeno, c1.prijmeni, c1.nazev_organu_cz, c1.pocet_volebnich_obdobi, c1.pocet_klubu
FROM ctx c1
WHERE c1.pocet_volebnich_obdobi >= ALL(
    SELECT c2.pocet_volebnich_obdobi
    FROM ctx c2
    WHERE c2.id_osoba = c1.id_osoba AND
          c2.nazev_organu_cz != c1.nazev_organu_cz
    )



