/*
Created: 5/10/2023
Model: DatabĂˇze hlasovĂˇnĂ­ v poslaneckĂ© snÄ›movnÄ›
Company: FEI VSB-TU Ostrava
Script author: Radim Baca
Database: MS SQL Server 2016
Source: https://www.psp.cz/sqw/hp.sqw?k=1301, https://www.psp.cz/sqw/hp.sqw?k=1302
*/

DROP TABLE IF EXISTS dbo.bod_schuze;
DROP TABLE IF EXISTS dbo.zmatecne;
DROP TABLE IF EXISTS dbo.hl_zposlanec;
DROP TABLE IF EXISTS dbo._schuze;
DROP TABLE IF EXISTS dbo.funkce;
DROP TABLE IF EXISTS dbo.typ_funkce;
DROP TABLE IF EXISTS dbo.omluvy;
DROP TABLE IF EXISTS dbo._omluvy;
DROP TABLE IF EXISTS dbo.hl_poslanec;
DROP TABLE IF EXISTS dbo.hl_hlasovani;
DROP TABLE IF EXISTS dbo._hl_hlasovani;
DROP TABLE IF EXISTS dbo.schuze_stav;
DROP TABLE IF EXISTS dbo.schuze;
DROP TABLE IF EXISTS dbo.poslanec;
DROP TABLE IF EXISTS dbo.organy;
DROP TABLE IF EXISTS dbo._organy;
DROP TABLE IF EXISTS dbo.typ_organu;
DROP TABLE IF EXISTS dbo.zarazeni;
DROP TABLE IF EXISTS dbo._zarazeni;
DROP TABLE IF EXISTS dbo.osoby;
DROP TABLE IF EXISTS dbo._osoby;
GO

CREATE TABLE dbo._osoby (
  id_osoba INT NOT NULL,
  pred VARCHAR(30) NULL,
  prijmeni VARCHAR(100) NULL,
  jmeno VARCHAR(100) NULL,
  za VARCHAR(255) NULL,
  narozeni VARCHAR(20) NULL,
  pohlavi VARCHAR(1) NULL,
  zmena NVARCHAR(255) NULL,
  umrti NVARCHAR(255) NULL,
  CONSTRAINT _osoby_pk PRIMARY KEY (id_osoba)
);

CREATE TABLE dbo.osoby (
  id_osoba INT NOT NULL,
  pred VARCHAR(30) NULL,
  prijmeni VARCHAR(100) NULL,
  jmeno VARCHAR(100) NULL,
  za VARCHAR(255) NULL,
  narozeni DATE NULL,
  pohlavi CHAR(1) NULL,
  zmena DATE NULL,
  umrti DATE NULL,
  CONSTRAINT osoby_pk PRIMARY KEY (id_osoba)
);

BULK INSERT _osoby
   FROM 'e:\d_ftp\geneea\osoby.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
	   ROWTERMINATOR = '\n',
	   CODEPAGE = 1250
      );



insert into osoby
select id_osoba, pred, prijmeni, jmeno, za,
             convert(date, narozeni, 104),
             pohlavi,
             convert(date, zmena, 104),
             convert(date, umrti, 104)
from _osoby
DROP TABLE IF EXISTS dbo._osoby;



------------------------------------------
DROP TABLE IF EXISTS dbo._zarazeni;
DROP TABLE IF EXISTS dbo.zarazeni;

CREATE TABLE dbo._zarazeni (
  id_osoba INT NULL,
  id_of INT NULL,
  cl_funkce INT NULL,
  od_o NVARCHAR(255) NULL,
  do_o NVARCHAR(255) NULL,
  od_f NVARCHAR(255) NULL,
  do_f NVARCHAR(255) NULL,
  CONSTRAINT _zarazeni_fk_id_osoba FOREIGN KEY (id_osoba) REFERENCES dbo.osoby (id_osoba) ON DELETE CASCADE ON UPDATE CASCADE
);

BULK INSERT _zarazeni
   FROM 'e:\d_ftp\geneea\zarazeni.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
	   ROWTERMINATOR = '\n',
	   CODEPAGE = 1250
      );

CREATE TABLE dbo.zarazeni (
  id_osoba INT NULL,
  id_of INT NULL,
  cl_funkce INT NULL,
  od_o DATE NULL,
  do_o DATE NULL,
  od_f DATE NULL,
  do_f DATE NULL,
  CONSTRAINT zarazeni_fk_id_osoba FOREIGN KEY (id_osoba) REFERENCES dbo.osoby (id_osoba) ON DELETE CASCADE ON UPDATE CASCADE
);


insert into zarazeni
select id_osoba, id_of, cl_funkce,
       convert(date, substring(od_o, 1, len(od_o)-2), 120),
       convert(date, substring(do_o, 1, len(do_o)-2), 120),
       convert(date, od_f, 104),
       convert(date, do_f, 104)
from _zarazeni
order by id_osoba, id_of, cl_funkce, od_o, do_o, od_f, do_f

drop table if exists dbo._zarazeni

-------------------------------------------------
DROP TABLE IF EXISTS dbo.typ_organu;
CREATE TABLE dbo.typ_organu (
  id_typ_org INT NOT NULL,
  typ_id_typ_org INT NULL,
  nazev_typ_org_cz NVARCHAR(255) NULL,
  nazev_typ_org_en NVARCHAR(255) NULL,
  typ_org_obecny INT NULL,
  priorita INT NULL,
  CONSTRAINT typ_organu_pk PRIMARY KEY (id_typ_org),
  CONSTRAINT typ_organu_fk_typ_id_typ_org FOREIGN KEY (typ_id_typ_org) REFERENCES dbo.typ_organu (id_typ_org)
);


BULK INSERT typ_organu
   FROM 'e:\d_ftp\geneea\typ_organu.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
	   ROWTERMINATOR = '\n',
	   CODEPAGE = 1250
      );

---------------------------------------
DROP TABLE IF EXISTS dbo._organy;
DROP TABLE IF EXISTS dbo.organy;
CREATE TABLE dbo._organy (
  id_organ INT NOT NULL,
  organ_id_organ INT NULL,
  id_typ_organu INT NULL,
  zkratka NVARCHAR(255) NULL,
  nazev_organu_cz NVARCHAR(MAX) NULL,
  nazev_organu_en NVARCHAR(MAX) NULL,
  od_organ NVARCHAR(255) NULL,
  do_organ NVARCHAR(255) NULL,
  priorita NVARCHAR(255) NULL,
  cl_organ_base INT NULL,
  CONSTRAINT _organy_pk PRIMARY KEY (id_organ),
  CONSTRAINT _organy_ibfk_1 FOREIGN KEY (id_typ_organu) REFERENCES dbo.typ_organu (id_typ_org) ON DELETE CASCADE ON UPDATE CASCADE
);
BULK INSERT _organy
   FROM 'e:\d_ftp\geneea\organy.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
       ROWTERMINATOR = '\n',
       CODEPAGE = 1250
      );

CREATE TABLE organy (
    id_organ INT NOT NULL,
    organ_id_organ INT NULL,
    id_typ_organu INT NULL,
    zkratka NVARCHAR(255) NULL,
    nazev_organu_cz NVARCHAR(MAX) NULL,
    nazev_organu_en NVARCHAR(MAX) NULL,
    od_organ DATE NULL,
    do_organ DATE NULL,
    priorita INT NULL,
    cl_organ_base INT NULL,
    CONSTRAINT organy_pk PRIMARY KEY (id_organ),
    CONSTRAINT organy_ibfk_1 FOREIGN KEY (id_typ_organu) REFERENCES dbo.typ_organu (id_typ_org) ON DELETE CASCADE ON UPDATE CASCADE
    );

insert into organy
select id_organ, organ_id_organ, id_typ_organu, zkratka, nazev_organu_cz, nazev_organu_en,
       convert(date, od_organ, 104),
       convert(date, do_organ, 104),
       priorita, cl_organ_base
from _organy
drop table if exists dbo._organy

-----------------------------
DROP TABLE IF EXISTS dbo.poslanec;
CREATE TABLE dbo.poslanec (
  id_poslanec INT NOT NULL,
  id_osoba INT NULL,
  id_kraj INT NULL,
  id_kandidatka INT NULL,
  id_obdobi INT NULL,
  web NVARCHAR(200) NULL,
  ulice VARCHAR(100) NULL,
  obec VARCHAR(100) NULL,
  psc VARCHAR(5) NULL,
  email VARCHAR(100) NULL,
  telefon VARCHAR(20) NULL,
  fax VARCHAR(20) NULL,
  psp_telefon VARCHAR(20) NULL,
  facebook VARCHAR(100) NULL,
  foto INT NULL,
  CONSTRAINT poslanec_pk PRIMARY KEY (id_poslanec),
  CONSTRAINT poslanec_fk_id_osoba FOREIGN KEY (id_osoba) REFERENCES osoby (id_osoba) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT poslanec_fk_id_obdobi FOREIGN KEY (id_obdobi) REFERENCES organy(id_organ) 
);


BULK INSERT poslanec
   FROM 'e:\d_ftp\geneea\poslanec.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
	   ROWTERMINATOR = '\n',
	   CODEPAGE = 1250
      );


--------------------------------------
DROP TABLE IF EXISTS dbo._schuze;
DROP TABLE IF EXISTS dbo.schuze;

CREATE TABLE dbo._schuze (
  id_schuze INT NOT NULL,
  id_organ INT NULL,
  schuze INT NULL,
  od_schuze VARCHAR(255) NULL,
  do_schuze VARCHAR(255) NULL,
  aktualizace VARCHAR(255) NULL,
  pozvanka INT NULL,
  CONSTRAINT schuze_ibfk_1 FOREIGN KEY (id_organ) REFERENCES dbo.organy (id_organ) ON DELETE CASCADE ON UPDATE CASCADE
);
BULK INSERT dbo._schuze FROM 'e:\d_ftp\geneea\schuze.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );

CREATE TABLE dbo.schuze (
  id_schuze INT NOT NULL,
  id_organ INT NULL,
  schuze INT NULL,
  od_schuze DATETIME2 NULL,
  do_schuze DATETIME2 NULL,
  aktualizace DATETIME2 NULL
  CONSTRAINT schuze_pk PRIMARY KEY (id_schuze),
  CONSTRAINT schuze_uniq UNIQUE(id_organ, schuze),
  CONSTRAINT schuze_fk_id_organ FOREIGN KEY (id_organ) REFERENCES dbo.organy (id_organ) ON DELETE CASCADE ON UPDATE CASCADE
);

INSERT INTO dbo.schuze (id_schuze, id_organ, schuze, od_schuze, do_schuze, aktualizace)
SELECT id_schuze, 
		MAX(id_organ), 
		MAX(schuze),
       CONVERT(DATETIME2, MAX(od_schuze), 120),
       CONVERT(DATETIME2, MAX(do_schuze), 120),
       CONVERT(DATETIME2, MAX(aktualizace), 120)
FROM dbo._schuze
GROUP BY id_schuze
DROP TABLE IF EXISTS dbo._schuze;


--------------------------------------
DROP TABLE IF EXISTS dbo.schuze_stav;
CREATE TABLE dbo.schuze_stav (
  id_schuze INT NULL,
  stav INT NULL,
  typ NVARCHAR(255) NULL,
  text_dt NVARCHAR(255) NULL,
  text_st NVARCHAR(255) NULL,
  tm_line NVARCHAR(255) NULL,
  CONSTRAINT schuze_stav_ibfk_1 FOREIGN KEY (id_schuze) REFERENCES dbo.schuze (id_schuze) ON DELETE CASCADE ON UPDATE CASCADE
);
BULK INSERT dbo.schuze_stav FROM 'e:\d_ftp\geneea\schuze_stav.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );


---------------------------------------
DROP TABLE IF EXISTS dbo._hl_hlasovani;
DROP TABLE IF EXISTS dbo.hl_hlasovani;
CREATE TABLE dbo._hl_hlasovani (
  id_hlasovani INT NOT NULL,
  id_organ INT NULL,
  schuze INT NULL,
  cislo INT NULL,
  bod INT NULL,
  datum VARCHAR(20) NULL,
  cas TIME NULL,
  pro INT NULL,
  proti INT NULL,
  zdrzel INT NULL,
  nehlasoval INT NULL,
  prihlaseno INT NULL,
  kvorum INT NULL,
  druh_hlasovani NVARCHAR(255) NULL,
  vysledek NVARCHAR(255) NULL,
  nazev_dlouhy NVARCHAR(MAX) NULL,
  nazev_kratky NVARCHAR(255) NULL,
  PRIMARY KEY (id_hlasovani),
  CONSTRAINT _hl_hlasovani_ibfk_1 FOREIGN KEY (id_organ) REFERENCES dbo.organy (id_organ) ON DELETE CASCADE ON UPDATE CASCADE
);
BULK INSERT _hl_hlasovani
   FROM 'e:\d_ftp\geneea\hl2002s.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
       ROWTERMINATOR = '\n',
       CODEPAGE = 1250
      );

BULK INSERT _hl_hlasovani
   FROM 'e:\d_ftp\geneea\hl2006s.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
       ROWTERMINATOR = '\n',
       CODEPAGE = 1250
      );

BULK INSERT _hl_hlasovani
   FROM 'e:\d_ftp\geneea\hl2010s.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
       ROWTERMINATOR = '\n',
       CODEPAGE = 1250
      );

BULK INSERT _hl_hlasovani
   FROM 'e:\d_ftp\geneea\hl2013s.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
       ROWTERMINATOR = '\n',
       CODEPAGE = 1250
      );

BULK INSERT _hl_hlasovani
   FROM 'e:\d_ftp\geneea\hl2017s.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
       ROWTERMINATOR = '\n',
       CODEPAGE = 1250
      );

BULK INSERT _hl_hlasovani
   FROM 'e:\d_ftp\geneea\hl2021s.unl'
   WITH
      (
       FIELDTERMINATOR = '|',
       ROWTERMINATOR = '\n',
       CODEPAGE = 1250
      );

CREATE TABLE hl_hlasovani (
    id_hlasovani INT NOT NULL,
    id_organ INT NULL,
    schuze INT NULL,
    cislo INT NULL,
    bod INT NULL,
    datum DATE NULL,
    cas TIME NULL,
    pro INT NULL,
    proti INT NULL,
    zdrzel INT NULL,
    nehlasoval INT NULL,
    prihlaseno INT NULL,
    kvorum INT NULL,
    druh_hlasovani CHAR(1) NULL,
    vysledek VARCHAR(7) NULL,
    nazev_dlouhy VARCHAR(MAX) NULL,
    nazev_kratky VARCHAR(255) NULL,
    PRIMARY KEY (id_hlasovani),
    CONSTRAINT hl_hlasovani_ibfk_1 FOREIGN KEY (id_organ) REFERENCES dbo.organy (id_organ) ON DELETE CASCADE ON UPDATE CASCADE,
	CONSTRAINT hl_hlasovani_schuzefk FOREIGN KEY (id_organ, schuze) REFERENCES dbo.schuze(id_organ, schuze)
    );

insert into hl_hlasovani
select id_hlasovani, id_organ, schuze, cislo, bod,
       convert(date, datum, 104),
       cas, pro, proti, zdrzel, nehlasoval, prihlaseno, kvorum, druh_hlasovani, vysledek, nazev_dlouhy, nazev_kratky
from _hl_hlasovani

DROP TABLE IF EXISTS dbo._hl_hlasovani;

---------------------------------------
DROP TABLE IF EXISTS dbo.hl_poslanec;
CREATE TABLE dbo.hl_poslanec (
  id_poslanec INT NOT NULL,
  id_hlasovani INT NOT NULL,
  vysledek CHAR(1) NULL,
  CONSTRAINT hl_poslanec_pk PRIMARY KEY (id_poslanec, id_hlasovani),  -- Composite primary key
  CONSTRAINT hl_poslanec_fk_id_poslanec FOREIGN KEY (id_poslanec) REFERENCES dbo.poslanec (id_poslanec) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT hl_poslanec_fk_id_hlasovani FOREIGN KEY (id_hlasovani) REFERENCES dbo.hl_hlasovani (id_hlasovani)
);

BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2002h1.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2002h2.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2002h3.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2006h1.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2006h2.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2010h1.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2010h2.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2013h1.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2013h2.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2017h1.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2017h2.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2021h1.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_poslanec FROM 'e:\d_ftp\geneea\hl2021h2.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );

---------------------------------------
DROP TABLE IF EXISTS dbo._omluvy;
DROP TABLE IF EXISTS dbo.omluvy;

CREATE TABLE dbo._omluvy (
  id_organ INT NOT NULL,
  id_poslanec INT NOT NULL,
  den NVARCHAR(255) NOT NULL,
  od NVARCHAR(255) NULL,
  do NVARCHAR(255) NULL,
  CONSTRAINT _omluvy_fk_id_organ FOREIGN KEY (id_organ) REFERENCES dbo.organy (id_organ) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT _omluvy_fk_id_poslanec FOREIGN KEY (id_poslanec) REFERENCES dbo.poslanec (id_poslanec)
);

BULK INSERT dbo._omluvy FROM 'e:\d_ftp\geneea\omluvy2002.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo._omluvy FROM 'e:\d_ftp\geneea\omluvy2006.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo._omluvy FROM 'e:\d_ftp\geneea\omluvy2010.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo._omluvy FROM 'e:\d_ftp\geneea\omluvy2013.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo._omluvy FROM 'e:\d_ftp\geneea\omluvy2017.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo._omluvy FROM 'e:\d_ftp\geneea\omluvy2021.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );

CREATE TABLE dbo.omluvy (
    id_organ INT NOT NULL,
    id_poslanec INT NOT NULL,
    den DATE NOT NULL,
    od TIME NULL,
    do TIME NULL,
    CONSTRAINT omluvy_fk_id_organ FOREIGN KEY (id_organ) REFERENCES dbo.organy (id_organ) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT omluvy_fk_id_poslanec FOREIGN KEY (id_poslanec) REFERENCES dbo.poslanec (id_poslanec) ON DELETE CASCADE ON UPDATE CASCADE
    );

INSERT INTO dbo.omluvy (id_organ, id_poslanec, den, od, do)
SELECT distinct id_organ, id_poslanec, CONVERT(DATE, den, 104), CAST(od AS TIME), CAST(do AS TIME)
FROM dbo._omluvy;

DROP TABLE IF EXISTS dbo._omluvy;

---------------------------------------
DROP TABLE IF EXISTS dbo.typ_funkce;

CREATE TABLE dbo.typ_funkce (
  id_typ_funkce INT NOT NULL,
  id_typ_org INT NULL,
  typ_funkce_cz VARCHAR(100) NULL,
  typ_funkce_en VARCHAR(100) NULL,
  priorita INT NULL,
  typ_funkce_obecny INT NULL,
  PRIMARY KEY (id_typ_funkce),
  CONSTRAINT typ_funkce_ibfk_1 FOREIGN KEY (id_typ_org) REFERENCES dbo.typ_organu (id_typ_org) ON DELETE CASCADE ON UPDATE CASCADE
);
BULK INSERT dbo.typ_funkce FROM 'e:\d_ftp\geneea\typ_funkce.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );

--------------------------------------
DROP TABLE IF EXISTS dbo.funkce;
CREATE TABLE dbo.funkce (
  id_funkce INT NOT NULL,
  id_organ INT NULL,
  id_typ_funkce INT NULL,
  nazev_funkce_cz VARCHAR(100) NULL,
  priorita INT NULL,
  CONSTRAINT funkce_pk PRIMARY KEY (id_funkce),
  CONSTRAINT funkce_fk_id_organ FOREIGN KEY (id_organ) REFERENCES dbo.organy (id_organ) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT funkce_fk_id_typ_funkce FOREIGN KEY (id_typ_funkce) REFERENCES dbo.typ_funkce (id_typ_funkce)
);
BULK INSERT dbo.funkce FROM 'e:\d_ftp\geneea\funkce.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );


--------------------------------------

DROP TABLE IF EXISTS dbo.hl_zposlanec;
CREATE TABLE dbo.hl_zposlanec (
  id_hlasovani INT NULL,
  id_osoba INT NULL,
  mode INT NULL,
  CONSTRAINT hl_zposlanec_ibfk_1 FOREIGN KEY (id_hlasovani) REFERENCES dbo.hl_hlasovani (id_hlasovani) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT hl_zposlanec_ibfk_2 FOREIGN KEY (id_osoba) REFERENCES dbo.osoby (id_osoba) ON DELETE CASCADE ON UPDATE CASCADE
);
BULK INSERT dbo.hl_zposlanec FROM 'e:\d_ftp\geneea\hl2010x.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_zposlanec FROM 'e:\d_ftp\geneea\hl2013x.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_zposlanec FROM 'e:\d_ftp\geneea\hl2017x.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.hl_zposlanec FROM 'e:\d_ftp\geneea\hl2021x.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );

--------------------------------------

DROP TABLE IF EXISTS dbo.zmatecne;
CREATE TABLE dbo.zmatecne (
  id_hlasovani INT NULL,
  CONSTRAINT zmatecne_ibfk_1 FOREIGN KEY (id_hlasovani) REFERENCES dbo.hl_hlasovani (id_hlasovani) ON DELETE CASCADE ON UPDATE CASCADE
);
BULK INSERT dbo.zmatecne FROM 'e:\d_ftp\geneea\zmatecne2002.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.zmatecne FROM 'e:\d_ftp\geneea\zmatecne2006.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.zmatecne FROM 'e:\d_ftp\geneea\zmatecne2010.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.zmatecne FROM 'e:\d_ftp\geneea\zmatecne2013.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.zmatecne FROM 'e:\d_ftp\geneea\zmatecne2017.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );
BULK INSERT dbo.zmatecne FROM 'e:\d_ftp\geneea\zmatecne2021.unl'
    WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );

	
--------------------------------------

DROP TABLE IF EXISTS dbo.bod_schuze;
CREATE TABLE dbo.bod_schuze (
  id_bod INT NOT NULL,
  id_schuze INT NOT NULL,
  id_tisk INT NULL,
  id_typ INT NULL,
  bod INT NOT NULL,
  uplny_naz NVARCHAR(MAX) NULL,
  uplny_kon NVARCHAR(255) NULL,
  poznamka NVARCHAR(255) NULL,
  id_bod_stav INT NOT NULL,
  pozvanka NVARCHAR(255) NULL,
  rj INT NULL,
  pozn2 NVARCHAR(255) NULL,
  druh_bodu INT NULL,
  id_sd INT NULL,
  zkratka NVARCHAR(50) NULL,
--PRIMARY KEY (id_bod, bod),
  CONSTRAINT bod_schuze_ibfk_1 FOREIGN KEY (id_schuze) REFERENCES dbo.schuze (id_schuze) ON DELETE CASCADE ON UPDATE CASCADE,
--  CONSTRAINT bod_schuze_ibfk_2 FOREIGN KEY (id_bod_stav) REFERENCES dbo.bod_stav (id_bod_stav) ON DELETE CASCADE ON UPDATE CASCADE
);
BULK INSERT dbo.bod_schuze FROM 'e:\d_ftp\geneea\bod_schuze.unl'
   WITH ( FIELDTERMINATOR = '|', ROWTERMINATOR = '\n', CODEPAGE = 1250 );