--1
CREATE TABLE PolitickaStrana (
    id_polstrana INT IDENTITY PRIMARY KEY,
    nazev VARCHAR(50)
)

INSERT INTO PolitickaStrana (nazev)
SELECT distinct org.zkratka AS 'nazev'
FROM organ org
JOIN typ_organu torg ON org.id_typ_org = torg.id_typ_org
WHERE torg.nazev_typ_org_cz = 'Klub'

--2
CREATE TABLE Clen (
    id_clen INT PRIMARY KEY,
    jmeno  VARCHAR(50),
    prijmeni VARCHAR(50)
)

INSERT INTO Clen (id_clen, jmeno, prijmeni)
SELECT distinct os.id_osoba AS 'id_clen', os.jmeno, os.prijmeni
FROM poslanec pos
JOIN osoba os ON os.id_osoba = pos.id_osoba
JOIN zarazeni z ON z.id_osoba = os.id_osoba
JOIN organ org ON org.id_organ = z.id_of AND z.cl_funkce = 0

--3
ALTER TABLE Clen ADD datum_narozeni DATE
UPDATE Clen SET datum_narozeni = (
    SELECT os.narozeni
    FROM osoba os
    WHERE os.id_osoba = id_clen
    )

SELECT COUNT(*) AS 'Pocet zaznamu'
FROM Clen c
WHERE EXISTS (
    SELECT 1
    FROM osoba os
    WHERE os.id_osoba = c.id_clen
) AND NOT EXISTS (
    SELECT 1
    FROM osoba os
    WHERE os.narozeni = c.datum_narozeni
)

--4
ALTER TABLE PolitickaStrana ADD datum_zalozeni DATE
ALTER TABLE PolitickaStrana ADD CONSTRAINT check_datum_zalozeni CHECK (datum_zalozeni >= '1990-01-01')

SELECT *
FROM PolitickaStrana
WHERE nazev = 'ODS' OR
      nazev = 'ODA'

UPDATE PolitickaStrana
SET datum_zalozeni = '1991-01-01'
WHERE nazev = 'ODS'

UPDATE PolitickaStrana
SET datum_zalozeni = '1970-01-01'
WHERE nazev = 'ODA'

SELECT *
FROM PolitickaStrana
WHERE nazev = 'ODS' OR
      nazev = 'ODA'

--5
ALTER TABLE Clen
DROP COLUMN datum_narozeni

SELECT *
FROM Clen
WHERE prijmeni = 'Kalousek'

DELETE FROM PolitickaStrana
WHERE nazev LIKE 'O%'

DROP TABLE Clen
DROP TABLE PolitickaStrana