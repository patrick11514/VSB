/*
Created: 16/10/2023
Model: The Chamber of Deputies, The Czech Republic
Company: FEI VSB-TU Ostrava
Script author for SQL Server: Radim Baca
Version for Oracle: Michal Fait, February 2024
Source: https://www.psp.cz/sqw/hp.sqw?k=1301, https://www.psp.cz/sqw/hp.sqw?k=1302
*/

create table osoba (
  id_osoba INT NOT NULL,
  pred VARCHAR(30) NULL,
  prijmeni VARCHAR(100) NULL,
  jmeno VARCHAR(100) NULL,
  za VARCHAR(255) NULL,
  narozeni DATE NULL,
  pohlavi CHAR(2) NULL,
  zmena DATE NULL,
  umrti DATE NULL,
  CONSTRAINT osoba_pk PRIMARY KEY (id_osoba)
);

create table osoba_ext 
(
      id_osoba INT NOT NULL,
      pred VARCHAR(30) NULL,
      prijmeni VARCHAR(100) NULL,
      jmeno VARCHAR(100) NULL,
      za VARCHAR(255) NULL,
      narozeni DATE NULL,
      pohlavi CHAR(2) NULL,
      zmena DATE NULL,
      umrti DATE NULL   
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_osoba CHAR(30),
      pred CHAR(30),
      prijmeni CHAR(100),
      jmeno CHAR(100),
      za CHAR(255),
      narozeni CHAR(100) date_format DATE mask "dd.mm.yyyy",
      pohlavi CHAR(2),
      zmena CHAR(100) date_format DATE mask "dd.mm.yyyy",
      umrti CHAR(100) date_format DATE mask "dd.mm.yyyy"   
    )
  )
  LOCATION ('osoby.unl')
)
REJECT LIMIT UNLIMITED;

insert into osoba select * from osoba_ext;

drop table osoba_ext;

---------------------------------------------------------------------------- 

create table zarazeni (
  id_osoba INT NULL,
  id_of INT NULL,
  cl_funkce INT NULL,
  od_o DATE NULL,
  do_o DATE NULL,
  od_f DATE NULL,
  do_f DATE NULL,
  CONSTRAINT zarazeni_fk_id_osoba FOREIGN KEY (id_osoba) REFERENCES osoba (id_osoba)
);

create table zarazeni_ext 
(
  id_osoba INT NULL,
  id_of INT NULL,
  cl_funkce INT NULL,
  od_o VARCHAR(100) NULL,
  do_o VARCHAR(100) NULL,
  od_f DATE NULL,
  do_f DATE NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_osoba CHAR(30),
      id_of CHAR(30),
      cl_funkce CHAR(30),
      od_o CHAR(100),
      do_o CHAR(100),
      od_f CHAR(100) date_format DATE mask "dd.mm.yyyy",
      do_f CHAR(100) date_format DATE mask "dd.mm.yyyy"
    )
  )
  LOCATION ('zarazeni.unl')
)
REJECT LIMIT UNLIMITED;

insert into zarazeni
select id_osoba, id_of, cl_funkce,
  TO_DATE (SUBSTR (od_o, 1, length(od_o)-2),'YYYY-MM-DD') as od_o,
  TO_DATE (SUBSTR (do_o, 1, length(do_o)-2),'YYYY-MM-DD') as do_o,
  od_f,
  do_f
from zarazeni_ext
order by id_osoba, id_of, cl_funkce, od_o, do_o, od_f, do_f;

drop table zarazeni_ext;

-------------------------------------------------------------------------------------

create table typ_organu (
  id_typ_org INT NOT NULL,
  rodic_id_typ_org INT NULL,
  nazev_typ_org_cz VARCHAR(255) NULL,
  nazev_typ_org_en VARCHAR(255) NULL,
  typ_org_obecny INT NULL,
  priorita INT NULL,
  CONSTRAINT typ_organu_pk PRIMARY KEY (id_typ_org),
  CONSTRAINT typ_organu_fk_rodic_id_typ_org FOREIGN KEY (rodic_id_typ_org) REFERENCES typ_organu (id_typ_org)
);

create table typ_organu_ext 
(
  id_typ_org INT NOT NULL,
  rodic_id_typ_org INT NULL,
  nazev_typ_org_cz VARCHAR(255) NULL,
  nazev_typ_org_en VARCHAR(255) NULL,
  typ_org_obecny INT NULL,
  priorita INT NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
        id_typ_org CHAR(30),
        rodic_id_typ_org CHAR(30),
        nazev_typ_org_cz CHAR(255),
        nazev_typ_org_en CHAR(255),
        typ_org_obecny CHAR(30),
        priorita CHAR(30)
    )
  )
  LOCATION ('typ_organu.unl')
)
REJECT LIMIT UNLIMITED;

insert into typ_organu (
  select id_typ_org, rodic_id_typ_org, nazev_typ_org_cz, nazev_typ_org_en, typ_org_obecny, priorita
  from typ_organu_ext A where exists (
    select * from typ_organu_ext B where A.rodic_id_typ_org = B.id_typ_org
  ) or a.rodic_id_typ_org is null
);

insert into typ_organu (
  select id_typ_org,null, nazev_typ_org_cz, nazev_typ_org_en, typ_org_obecny, priorita
  from typ_organu_ext A where not exists (
    select * from typ_organu_ext B where A.rodic_id_typ_org = B.id_typ_org
  ) and a.rodic_id_typ_org is not null
);

drop table typ_organu_ext;

---------------------------------------------------------------------------------

create table organ (
  id_organ INT NOT NULL,
  rodic_id_organ INT NULL,
  id_typ_org INT NULL,
  zkratka VARCHAR(255) NULL,
  nazev_organu_cz VARCHAR(4000) NULL,
  nazev_organu_en VARCHAR(4000) NULL,
  od_organ DATE NULL,
  do_organ DATE NULL,
  priorita INT NULL,
  cl_organ_base INT NULL,
  CONSTRAINT organ_pk PRIMARY KEY (id_organ),
  CONSTRAINT organ_fk_rodic_id_organ FOREIGN KEY (rodic_id_organ) REFERENCES organ(id_organ),
  CONSTRAINT organ_fk_id_typ_org FOREIGN KEY (id_typ_org) REFERENCES typ_organu (id_typ_org)
);
    
create table organ_ext
(
  id_organ INT NOT NULL,
  rodic_id_organ INT NULL,
  id_typ_org INT NULL,
  zkratka VARCHAR(255) NULL,
  nazev_organu_cz VARCHAR(4000) NULL,
  nazev_organu_en VARCHAR(4000) NULL,
  od_organ DATE NULL,
  do_organ DATE NULL,
  priorita INT NULL,
  cl_organ_base INT NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_organ CHAR(30),
      rodic_id_organ CHAR(30),
      id_typ_org CHAR(30),
      zkratka CHAR(255),
      nazev_organu_cz CHAR(4000),
      nazev_organu_en CHAR(4000),
      od_organ CHAR(100) date_format DATE mask "dd.mm.yyyy",
      do_organ CHAR(100) date_format DATE mask "dd.mm.yyyy",
      priorita CHAR(30),
      cl_organ_base CHAR(30)
    )
  )
  LOCATION ('organy.unl')
)
REJECT LIMIT UNLIMITED;

insert into organ
  select * from organ_ext A where exists (
    select * from organ_ext B where A.rodic_id_organ = B.id_organ
  ) or A.rodic_id_organ is null;

insert into organ
  select id_organ, null, id_typ_org, zkratka,nazev_organu_cz,nazev_organu_en, od_organ, do_organ, priorita, cl_organ_base 
  from organ_ext A where not exists (
    select * from organ_ext B where A.rodic_id_organ = B.id_organ
  ) and A.rodic_id_organ is not null;

drop table organ_ext;

---------------------------------------------------------------------------------------

create table poslanec (
  id_poslanec INT NOT NULL,
  id_osoba INT NULL,
  id_kraj INT NULL,
  id_kandidatka INT NULL,
  id_organ INT NULL,
  web VARCHAR(200) NULL,
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
  CONSTRAINT poslanec_fk_id_osoba FOREIGN KEY (id_osoba) REFERENCES osoba (id_osoba),
  CONSTRAINT poslanec_fk_id_organ FOREIGN KEY (id_organ) REFERENCES organ(id_organ) 
);

create table poslanec_ext
(
  id_poslanec INT NOT NULL,
  id_osoba INT NULL,
  id_kraj INT NULL,
  id_kandidatka INT NULL,
  id_organ INT NULL,
  web VARCHAR(200) NULL,
  ulice VARCHAR(100) NULL,
  obec VARCHAR(100) NULL,
  psc VARCHAR(5) NULL,
  email VARCHAR(100) NULL,
  telefon VARCHAR(20) NULL,
  fax VARCHAR(20) NULL,
  psp_telefon VARCHAR(20) NULL,
  facebook VARCHAR(100) NULL,
  foto INT NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_poslanec CHAR(30),
      id_osoba CHAR(30),
      id_kraj CHAR(30),
      id_kandidatka CHAR(30),
      id_organ CHAR(30),
      web CHAR(200),
      ulice CHAR(100),
      obec CHAR(100),
      psc CHAR(5),
      email CHAR(100),
      telefon CHAR(20),
      fax CHAR(20),
      psp_telefon CHAR(20),
      facebook CHAR(100),
      foto CHAR(30)
    )
  )
  LOCATION ('poslanec.unl')
)
REJECT LIMIT UNLIMITED;

insert into poslanec select * from poslanec_ext;

drop table poslanec_ext;

-----------------------------------------------------------------------------

create table schuze (
  id_schuze INT NOT NULL,
  id_organ INT NULL,
  schuze INT NULL,
  od_schuze TIMESTAMP NULL,
  do_schuze TIMESTAMP NULL,
  aktualizace TIMESTAMP NULL,
  CONSTRAINT schuze_pk PRIMARY KEY (id_schuze),
  CONSTRAINT schuze_uniq UNIQUE(id_organ, schuze),
  CONSTRAINT schuze_fk_id_organ FOREIGN KEY (id_organ) REFERENCES organ (id_organ)
);

create table schuze_ext
(
  id_schuze INT NOT NULL,
  id_organ INT NULL,
  schuze INT NULL,
  od_schuze TIMESTAMP NULL,
  do_schuze TIMESTAMP NULL,
  aktualizace TIMESTAMP NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_schuze char(30),
      id_organ char(30),
      schuze char(30),
      od_schuze char(100) DATE_FORMAT TIMESTAMP MASK "yyyy-mm-dd hh24:mi",
      do_schuze char(100) DATE_FORMAT TIMESTAMP MASK "yyyy-mm-dd hh24:mi",
      aktualizace char(100) DATE_FORMAT TIMESTAMP MASK "yyyy-mm-dd hh24:mi"
    )
  )
  LOCATION ('schuze.unl')
)
REJECT LIMIT UNLIMITED;

insert into schuze (id_schuze, id_organ, schuze, od_schuze, do_schuze, aktualizace)
  select id_schuze, max(id_organ), max(schuze), max(od_schuze), max(do_schuze), max(aktualizace)
from schuze_ext
group by id_schuze;

drop table schuze_ext;

-------------------------------------------------------------------------------------

create table schuze_stav (
  id_schuze INT NULL,
  stav INT NULL,
  typ VARCHAR(255) NULL,
  text_dt VARCHAR(255) NULL,
  text_st VARCHAR(255) NULL,
  tm_line VARCHAR(255) NULL,
  CONSTRAINT schuze_stav_fk_id_schuze FOREIGN KEY (id_schuze) REFERENCES schuze (id_schuze)
);

create table schuze_stav_ext
(
  id_schuze INT NULL,
  stav INT NULL,
  typ VARCHAR(255) NULL,
  text_dt VARCHAR(255) NULL,
  text_st VARCHAR(255) NULL,
  tm_line VARCHAR(255) NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_schuze char(30),
      stav char(30),
      typ CHAR(255),
      text_dt CHAR(255),
      text_st CHAR(255),
      tm_line CHAR(255)
    )
  )
  LOCATION ('schuze_stav.unl')
)
REJECT LIMIT UNLIMITED;

insert into schuze_stav 
  select * from schuze_stav_ext SS where exists (
    select * from schuze S where S.id_schuze = SS.id_schuze
  );

insert into schuze_stav 
  select null,stav,typ,text_dt, text_st,tm_line from schuze_stav_ext SS where not exists (
    select * from schuze S where S.id_schuze = SS.id_schuze
  );

drop table schuze_stav_ext;

------------------------------------------------------------------------------------------

create table hlasovani (
  id_hlasovani INT NOT NULL,
  id_organ INT NULL,
  schuze INT NULL,
  cislo INT NULL,
  bod INT NULL,
  datum DATE NULL,
  cas TIMESTAMP NULL,
  pro INT NULL,
  proti INT NULL,
  zdrzel INT NULL,
  nehlasoval INT NULL,
  prihlaseno INT NULL,
  kvorum INT NULL,
  druh_hlasovani CHAR(1) NULL,
  vysledek VARCHAR(7) NULL,
  nazev_dlouhy VARCHAR(4000) NULL,
  nazev_kratky VARCHAR(255) NULL,
  PRIMARY KEY (id_hlasovani),
  CONSTRAINT hlasovani_fk_id_organ FOREIGN KEY (id_organ) REFERENCES organ (id_organ),
  CONSTRAINT hlasovani_schuze_fk FOREIGN KEY (id_organ, schuze) REFERENCES schuze(id_organ, schuze)
);

create table hlasovani_ext
(
  id_hlasovani INT NOT NULL,
  id_organ INT NULL,
  schuze INT NULL,
  cislo INT NULL,
  bod INT NULL,
  datum DATE NULL,
  cas TIMESTAMP NULL,
  pro INT NULL,
  proti INT NULL,
  zdrzel INT NULL,
  nehlasoval INT NULL,
  prihlaseno INT NULL,
  kvorum INT NULL,
  druh_hlasovani CHAR(1) NULL,
  vysledek VARCHAR(7) NULL,
  nazev_dlouhy VARCHAR(4000) NULL,
  nazev_kratky VARCHAR(255) NULL 
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_hlasovani CHAR(30),
      id_organ CHAR(30),
      schuze CHAR(30),
      cislo CHAR(30),
      bod CHAR(30),
      datum CHAR(100) date_format DATE mask "dd.mm.yyyy",
      cas CHAR(100) DATE_FORMAT TIMESTAMP MASK "hh24:mi",
      pro CHAR(30),
      proti CHAR(30),
      zdrzel CHAR(30),
      nehlasoval CHAR(30),
      prihlaseno CHAR(30),
      kvorum CHAR(30),
      druh_hlasovani CHAR(1),
      vysledek CHAR(7),
      nazev_dlouhy CHAR(4000),
      nazev_kratky CHAR(255)
    )
  )
  LOCATION ('hl2002s.unl','hl2006s.unl','hl2010s.unl','hl2013s.unl','hl2017s.unl','hl2021s.unl')
)
REJECT LIMIT UNLIMITED;

insert into hlasovani 
  select * from hlasovani_ext;

drop table hlasovani_ext;

------------------------------------------------------------------------------------------

create table hlasovani_poslanec (
  id_poslanec INT NOT NULL,
  id_hlasovani INT NOT NULL,
  vysledek CHAR(1) NULL,
  CONSTRAINT hlasovani_poslanec_pk PRIMARY KEY (id_poslanec, id_hlasovani),  -- Composite primary key
  CONSTRAINT hlasovani_poslanec_fk_id_poslanec FOREIGN KEY (id_poslanec) REFERENCES poslanec (id_poslanec),
  CONSTRAINT hlasovani_poslanec_fk_id_hlasovani FOREIGN KEY (id_hlasovani) REFERENCES hlasovani (id_hlasovani)
);

create table hlasovani_poslanec_ext
(
  id_poslanec INT NOT NULL,
  id_hlasovani INT NOT NULL,
  vysledek CHAR(1) NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_poslanec char(30),
      id_hlasovani char(30),
      vysledek char(1)
    )
  )
  LOCATION ('hl2002h1.unl','hl2002h2.unl','hl2002h3.unl','hl2006h1.unl','hl2006h2.unl','hl2010h1.unl','hl2010h2.unl','hl2013h1.unl','hl2013h2.unl','hl2017h1.unl','hl2017h2.unl','hl2021h1.unl','hl2021h2.unl')
)
REJECT LIMIT UNLIMITED;

insert into hlasovani_poslanec 
  select * from hlasovani_poslanec_ext;
  
drop table hlasovani_poslanec_ext;

------------------------------------------------------------------------------------------

create table omluva (
  id_organ INT NOT NULL,
  id_poslanec INT NOT NULL,
  den DATE NOT NULL,
  od TIMESTAMP NULL,
  do TIMESTAMP NULL,
  CONSTRAINT omluva_fk_id_organ FOREIGN KEY (id_organ) REFERENCES organ (id_organ),
  CONSTRAINT omluva_fk_id_poslanec FOREIGN KEY (id_poslanec) REFERENCES poslanec (id_poslanec)
);
    
create table omluva_ext
(
  id_organ INT NOT NULL,
  id_poslanec INT NOT NULL,
  den DATE NOT NULL,
  od TIMESTAMP NULL,
  do TIMESTAMP NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
        id_organ char(30),
        id_poslanec char(30) ,
        den char(100) date_format DATE mask "dd.mm.yyyy",
        od CHAR(100) DATE_FORMAT TIMESTAMP MASK "hh24:mi",
        do CHAR(100) DATE_FORMAT TIMESTAMP MASK "hh24:mi"
    )
  )
  LOCATION ('omluvy2002.unl','omluvy2006.unl','omluvy2010.unl','omluvy2013.unl','omluvy2017.unl','omluvy2021.unl')
)
REJECT LIMIT UNLIMITED;

insert into omluva
  select distinct id_organ, id_poslanec,  den, od , do
  from omluva_ext;

drop table omluva_ext;

------------------------------------------------------------------------------------------

create table typ_funkce (
  id_typ_funkce INT NOT NULL,
  id_typ_org INT NULL,
  typ_funkce_cz VARCHAR(100) NULL,
  typ_funkce_en VARCHAR(100) NULL,
  priorita INT NULL,
  typ_funkce_obecny INT NULL,
  PRIMARY KEY (id_typ_funkce),
  CONSTRAINT typ_funkce_ibfk_1 FOREIGN KEY (id_typ_org) REFERENCES typ_organu (id_typ_org)
);
  
create table typ_funkce_ext
(
  id_typ_funkce INT NOT NULL,
  id_typ_org INT NULL,
  typ_funkce_cz VARCHAR(100) NULL,
  typ_funkce_en VARCHAR(100) NULL,
  priorita INT NULL,
  typ_funkce_obecny INT NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_typ_funkce char(30),
      id_typ_org char(30),
      typ_funkce_cz CHAR(100),
      typ_funkce_en CHAR(100),
      priorita char(30),
      typ_funkce_obecny char(30)
    )
  )
  LOCATION ('typ_funkce.unl')
)
REJECT LIMIT UNLIMITED;

insert into typ_funkce 
  select * from typ_funkce_ext;

drop table typ_funkce_ext;

------------------------------------------------------------------------------------------

create table funkce (
  id_funkce INT NOT NULL,
  id_organ INT NULL,
  id_typ_funkce INT NULL,
  nazev_funkce_cz VARCHAR(100) NULL,
  priorita INT NULL,
  CONSTRAINT funkce_pk PRIMARY KEY (id_funkce),
  CONSTRAINT funkce_fk_id_organ FOREIGN KEY (id_organ) REFERENCES organ (id_organ),
  CONSTRAINT funkce_fk_id_typ_funkce FOREIGN KEY (id_typ_funkce) REFERENCES typ_funkce (id_typ_funkce)
);

create table funkce_ext 
(
  id_funkce INT NOT NULL,
  id_organ INT NULL,
  id_typ_funkce INT NULL,
  nazev_funkce_cz VARCHAR(100) NULL,
  priorita INT NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_funkce char(30),
      id_organ char(30),
      id_typ_funkce char(30),
      nazev_funkce_cz CHAR(100),
      priorita char(30)
    )
  )
  LOCATION ('funkce.unl')
)
REJECT LIMIT UNLIMITED;

insert into funkce
  select * from funkce_ext;

drop table funkce_ext;

------------------------------------------------------------------------------------------

create table zmatecne_hlasovani_poslanec (
  id_hlasovani INT NULL,
  id_osoba INT NULL,
  mode_ INT NULL,
  CONSTRAINT zmatecne_hlasovani_poslanec_ibfk_1 FOREIGN KEY (id_hlasovani) REFERENCES hlasovani (id_hlasovani),
  CONSTRAINT zmatecne_hlasovani_poslanec_ibfk_2 FOREIGN KEY (id_osoba) REFERENCES osoba (id_osoba)
);

create table zmatecne_hlasovani_poslanec_ext
(
  id_hlasovani INT NULL,
  id_osoba INT NULL,
  mode_ INT NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
        id_hlasovani char(30),
        id_osoba char(30),
        mode_ char(30)
    )
  )
  LOCATION ('hl2010x.unl','hl2013x.unl','hl2017x.unl','hl2021x.unl')
)
REJECT LIMIT UNLIMITED;

insert into zmatecne_hlasovani_poslanec 
  select * from zmatecne_hlasovani_poslanec_ext;

drop table zmatecne_hlasovani_poslanec_ext;

------------------------------------------------------------------------------------------

create table zmatecne (
  id_hlasovani INT NULL,
  CONSTRAINT zmatecne_ibfk_1 FOREIGN KEY (id_hlasovani) REFERENCES hlasovani (id_hlasovani)
);

create table zmatecne_ext
(
  id_hlasovani INT NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
        id_hlasovani char(30)
    )
  )
  LOCATION ('zmatecne2002.unl','zmatecne2006.unl','zmatecne2010.unl','zmatecne2013.unl','zmatecne2017.unl','zmatecne2021.unl')
)
REJECT LIMIT UNLIMITED;

insert into zmatecne
  select * from zmatecne_ext Z where exists (select * from hlasovani H where z.id_hlasovani = h.id_hlasovani); 

drop table zmatecne_ext;

------------------------------------------------------------------------------------------

create table bod_schuze (
  id_bod INT NOT NULL,
  id_schuze INT NOT NULL,
  id_tisk INT NULL,
  id_typ INT NULL,
  bod INT NOT NULL,
  uplny_naz VARCHAR(4000) NULL,
  uplny_kon VARCHAR(255) NULL,
  poznamka VARCHAR(255) NULL,
  id_bod_stav INT NOT NULL,
  pozvanka VARCHAR(255) NULL,
  rj INT NULL,
  pozn2 VARCHAR(255) NULL,
  druh_bodu INT NULL,
  id_sd INT NULL,
  zkratka VARCHAR(50) NULL,
  CONSTRAINT bod_schuze_ibfk_1 FOREIGN KEY (id_schuze) REFERENCES schuze (id_schuze)
);

create table bod_schuze_ext
(
  id_bod INT NOT NULL,
  id_schuze INT NOT NULL,
  id_tisk INT NULL,
  id_typ INT NULL,
  bod INT NOT NULL,
  uplny_naz VARCHAR(4000) NULL,
  uplny_kon VARCHAR(255) NULL,
  poznamka VARCHAR(255) NULL,
  id_bod_stav INT NOT NULL,
  pozvanka VARCHAR(255) NULL,
  rj INT NULL,
  pozn2 VARCHAR(255) NULL,
  druh_bodu INT NULL,
  id_sd INT NULL,
  zkratka VARCHAR(50) NULL
)
organization external
(
  TYPE ORACLE_LOADER
  DEFAULT DIRECTORY DATA_FTP_GENEEA
  ACCESS PARAMETERS
  (
    RECORDS DELIMITED BY NEWLINE
    CHARACTERSET EE8MSWIN1250
    FIELDS TERMINATED BY '|'
    MISSING FIELD VALUES ARE NULL 
    (
      id_bod char(30),
      id_schuze char(30),
      id_tisk char(30),
      id_typ char(30),
      bod char(30),
      uplny_naz CHAR(4000),
      uplny_kon CHAR(255),
      poznamka CHAR(255),
      id_bod_stav char(30),
      pozvanka CHAR(255),
      rj char(30),
      pozn2 CHAR(255),
      druh_bodu char(30),
      id_sd char(30),
      zkratka CHAR(50)
    )
  )
  LOCATION ('bod_schuze.unl')
)
REJECT LIMIT UNLIMITED;

insert into bod_schuze
  select * from bod_schuze_ext;

drop table bod_schuze_ext;

commit;

