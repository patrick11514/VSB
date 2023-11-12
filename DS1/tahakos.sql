-- Poznámka: Žádné ze zde uvedených řešení si nenárokuje titul "jediné správné řešení".
-- Vždy existuje mnoho variant zápisu a ideálně byste měli být schopni to zapsat i jinak než je uvedeno v řešení

----------------------------------------------------
----------------------------------------------------
-- Útery 12:30

-- A

-------
-- 1. Pro každé volební období nalezněte osobu, která prohlásila nejvíce různých hlasování za zmatečné (informace o tom naleznete v tabulce zmatecne_hlasovani_poslanec).
-- Vypište id_osoba, jméno, příjmení, id_organ daného volebního období a počet různých hlasování, které v období prohlásila osoba za zmatečné.
with tab as (
	select osoba.id_osoba, osoba.jmeno, osoba.prijmeni, schuze.id_organ, count(distinct zhp.id_hlasovani) cnt
	from schuze
	join hlasovani on schuze.schuze = hlasovani.schuze and
					  schuze.id_organ = hlasovani.id_organ
	join zmatecne_hlasovani_poslanec zhp on hlasovani.id_hlasovani = zhp.id_hlasovani
	join osoba on osoba.id_osoba = zhp.id_osoba
	group by osoba.id_osoba, osoba.jmeno, osoba.prijmeni, schuze.id_organ
)
select *
from tab t1
where cnt >= all (
	select t2.cnt
	from tab t2
	where t2.id_organ = t1.id_organ
)
order by id_organ


WITH POCTY AS (
	SELECT o.id_osoba, o.jmeno, o.prijmeni, hl.id_organ, COUNT(CASE WHEN hlp.mode = 1 THEN 1 END) as pocty
	FROM schuze s
	JOIN hlasovani hl ON s.id_organ = hl.id_organ AND s.schuze = hl.schuze
	JOIN zmatecne_hlasovani_poslanec hlp ON hlp.id_hlasovani = hl.id_hlasovani
	JOIN osoba o ON hlp.id_osoba = o.id_osoba
	GROUP BY o.id_osoba, o.jmeno, o.prijmeni, hl.id_organ
)
SELECT *
FROM POCTY p1 WHERE p1.pocty >= ALL (SELECT p2.pocty FROM POCTY p2 WHERE p1.id_organ = p2.id_organ)
ORDER BY p1.id_organ

-------------
-- 2. Napište dotaz, který pro hlasování konané 24.10.2019 (hlasovani.datum = '2019-10-24') s číslem 172 vypíše následující údaje:
-- 1. id_hlasovani, datum, nazev_dlouhy
-- 2. kolik poslanců ODS hlasovalo PRO,
-- 3. kolik poslanců strany ANO hlasovalo PRO,
-- Hlasování PRO se pozná tak, že v hlasovani_poslanec.vysledek = 'A'.
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

-- varianta s conditional aggregation
SELECT hh.id_hlasovani, hh.datum, hh.nazev_dlouhy,
	count(CASE WHEN org.zkratka = 'ANO' and hp.vysledek = 'A' THEN 1 END) Ano_pro,
	count(CASE WHEN org.zkratka = 'ODS' and hp.vysledek = 'A' THEN 1 END) ODS_pro
FROM poslanec p
JOIN zarazeni z ON p.id_osoba = z.id_osoba and z.cl_funkce = 0
JOIN organ org ON z.id_of = org.id_organ
JOIN organ psp2013 ON psp2013.id_organ = p.id_organ and org.rodic_id_organ = psp2013.id_organ
JOIN hlasovani_poslanec hp ON hp.id_poslanec = p.id_poslanec
JOIN hlasovani hh ON hh.id_hlasovani = hp.id_hlasovani
WHERE psp2013.nazev_organu_cz='Poslanecká sněmovna' and
		YEAR(psp2013.od_organ) = 2017

-- varianta s conditional aggregation
SELECT hh.id_hlasovani, hh.datum, hh.nazev_dlouhy,
	count(CASE WHEN org.zkratka = 'ANO' and hp.vysledek = 'A' THEN 1 END) Ano_pro,
	count(CASE WHEN org.zkratka = 'ODS' and hp.vysledek = 'A' THEN 1 END) ODS_pro
FROM poslanec p
JOIN zarazeni z ON p.id_osoba = z.id_osoba and z.cl_funkce = 0
JOIN organ org ON z.id_of = org.id_organ
JOIN organ psp2013 ON psp2013.id_organ = p.id_organ and org.rodic_id_organ = psp2013.id_organ
JOIN hlasovani_poslanec hp ON hp.id_poslanec = p.id_poslanec
JOIN hlasovani hh ON hh.id_hlasovani = hp.id_hlasovani
WHERE psp2013.nazev_organu_cz='Poslanecká sněmovna' and
		YEAR(psp2013.od_organ) = 2017 and
		hh.datum = '2019-10-24' and
		cislo = 172
GROUP BY hh.id_hlasovani, hh.datum, hh.nazev_dlouhy, hh.pro, hh.proti, hh.prihlaseno

----------
-- 3. Nalezněte schůze (id_schuze, schuze, od_schuze) zasahující do roku 2015, ve kterých se nekonalo ani jedno zmatečné hlasování.
-- Vynechte schůze, ve kterých neproběhlo ani jedno hlasování.
-- Seznam zmatečných hlasování naleznete v tabulce zmatecne.
-- Setřiďte podle id_schuze.
select id_schuze, schuze, od_schuze
from schuze s
where not exists (
  select 1
  from hlasovani hh
  join zmatecne z on z.id_hlasovani = hh.id_hlasovani
  where s.schuze = hh.schuze and
        s.id_organ = hh.id_organ
) and exists (
  select 1
  from hlasovani hh
  where s.schuze = hh.schuze and
        s.id_organ = hh.id_organ
) and (year(s.od_schuze) = 2015 or year(s.do_schuze) = 2015)
order by id_schuze

-- B

-------
-- 1. Pro každé volební období nalezněte osobu, která prohlásila nejvíce různých hlasování za zmatečné (informace o tom naleznete v tabulce zmatecne_hlasovani_poslanec).
-- Vypište id_osoba, jméno, příjmení, id_organ daného volebního období a počet různých hlasování, které v období prohlásila osoba za zmatečné.
WITH tab AS (
	select osoba.id_osoba, osoba.jmeno, osoba.prijmeni, schuze.id_organ, count(distinct zhp.id_hlasovani) cnt
	from schuze
	join hlasovani on schuze.schuze = hlasovani.schuze and
					  schuze.id_organ = hlasovani.id_organ
	join zmatecne_hlasovani_poslanec zhp on hlasovani.id_hlasovani = zhp.id_hlasovani
	join osoba on osoba.id_osoba = zhp.id_osoba
	group by osoba.id_osoba, osoba.jmeno, osoba.prijmeni, schuze.id_organ
)
select *
from tab t1
where cnt >= all (
	select t2.cnt
	from tab t2
	where t2.id_organ = t1.id_organ
)
order by id_organ


-------------
-- 2. Napište dotaz, který pro hlasování konané 29.9.2020 (hlasovani.datum = '2020-09-29') s číslem 249 (tj hlasovani.cislo) vypíše následující údaje:
-- 1. id_hlasovani, datum, nazev_dlouhy
-- 2. kolik poslanců ODS hlasovalo PRO,
-- 3. kolik poslanců strany ANO hlasovalo PRO,
-- Hlasování PRO se pozná tak, že v hlasovani_poslanec.vysledek = 'A'.
SELECT hh.id_hlasovani, hh.datum, hh.nazev_dlouhy, hh.cislo,
	count(CASE WHEN org.zkratka = 'ODS' and hp.vysledek = 'A' THEN 1 END) SPD_pro,
	count(CASE WHEN org.zkratka = 'ANO' and hp.vysledek = 'A' THEN 1 END) ODS_pro
FROM poslanec p
JOIN zarazeni z ON p.id_osoba = z.id_osoba and z.cl_funkce = 0
JOIN organ org ON z.id_of = org.id_organ
JOIN organ psp2013 ON psp2013.id_organ = p.id_organ and org.rodic_id_organ = psp2013.id_organ
JOIN hlasovani_poslanec hp ON hp.id_poslanec = p.id_poslanec
JOIN hlasovani hh ON hh.id_hlasovani = hp.id_hlasovani
WHERE psp2013.nazev_organu_cz='Poslanecká sněmovna' and
		YEAR(psp2013.od_organ) = 2017 and
		hh.datum = '2020-09-29' and
		cislo = 249
GROUP BY hh.id_hlasovani, hh.datum, hh.nazev_dlouhy, hh.cislo


----------
-- 3. Nalezněte schůze (id_schuze, schuze, od_schuze) zasahující do roku 2020, ve kterých se nekonalo ani jedno zmatečné hlasování.
-- Vynechte schůze, ve kterých neproběhlo ani jedno hlasování.
-- Seznam zmatečných hlasování naleznete v tabulce zmatecne.
-- Setřiďte podle id_schuze.
select id_schuze, schuze, od_schuze
from schuze s
where not exists (
  select 1
  from hlasovani hh
  join zmatecne z on z.id_hlasovani = hh.id_hlasovani
  where s.schuze = hh.schuze and
        s.id_organ = hh.id_organ
) and exists (
  select 1
  from hlasovani hh
  where s.schuze = hh.schuze and
        s.id_organ = hh.id_organ
) and (year(s.od_schuze) = 2020 or year(s.do_schuze) = 2020)
order by id_schuze





-- Úterý 14:15

-- A

-------------
-- 1. Nalezněte poslanecké kluby, které měly dvě a více volebních období v Poslanecké sněmovně minimálně 50 poslanců
-- (stačí počet záznamů v tabulce poslanec pro dané období).
-- Vypište zkratku poslaneckého klubu a počet volebních období, kdy měl poslanecký klub více než 50 poslanců.
-- Poslanecký klub je záznam v organ pro který typ_organu.nazev_typ_org_cz = 'klub'.
SELECT t.zkratka, count(*)
FROM (
	SELECT org.zkratka, org.nazev_organu_cz, psp2013.od_organ, count(*) cnt
	FROM poslanec p
	JOIN zarazeni z ON p.id_osoba = z.id_osoba and z.cl_funkce = 0
	JOIN organ org ON z.id_of = org.id_organ
	JOIN typ_organu typo ON org.id_typ_org = typo.id_typ_org
	JOIN organ psp2013 ON psp2013.id_organ = p.id_organ and org.rodic_id_organ = psp2013.id_organ
	WHERE typo.nazev_typ_org_cz = 'klub'
	GROUP BY org.zkratka, psp2013.od_organ, org.nazev_organu_cz
	HAVING count(*) >= 50
) t
GROUP BY t.zkratka
HAVING count(*) >= 2



------
-- 2. Pro každou schůzi ve volebním období 2013 - 2017 nalezněte poslední hlasování.
-- K porovnání pořadí můžete využít atribut hlasovani.cislo.
-- Tzn. zajímá nás hlasování s nejvyšší hodnotou atributu hlasovani.cislo v rámci jedné schůze.
-- Setřiďte výsledek podle atributu schuze.schuze a vypište atributy id_hlasovani, schuze, cislo a schuze.nazev_dlouhy.
with tab as (
	select hlasovani.*
	from schuze
	join hlasovani on schuze.schuze = hlasovani.schuze and
						schuze.id_organ = hlasovani.id_organ
	join organ on organ.id_organ = schuze.id_organ
	where year(organ.od_organ) = 2013
)
select id_hlasovani, schuze, cislo, nazev_dlouhy
from tab t1
where t1.cislo >= all (
  select cislo
  from tab t2
  where t1.schuze = t2.schuze
)
order by t1.schuze

select s.id_schuze,h.id_hlasovani, s.schuze, h.cislo, h.nazev_dlouhy
from schuze s
JOIN organ on s.id_organ = organ.id_organ and YEAR(organ.od_organ) = 2013
JOIN hlasovani h on s.schuze = h.schuze and h.id_organ = s.id_organ
where h.cislo = (
	select MAX(h2.cislo)
	from hlasovani h2
	JOIN schuze s2 on h2.schuze = s2.schuze and h2.id_organ = s2.id_organ
	where s2.id_schuze = s.id_schuze and s2.id_organ = s.id_organ
)
order by s.schuze



------
-- 3. Nalezněte osoby, které nikdy ve volebním období 2017 - 2021 nehlasovaly PRO
-- (hlasovani_poslanec.vysledek = 'A') u hlasování,
-- jehož dlouhý název řetězce začíná "Návrh na vyslovení nedůvěry vládě" (tj. hlasovani.nazev_dlouhy).
-- Vyberte pouze osoby, které hlasovaly v nějakém takovém hlasování, ale nikdy nehlasovaly pro.
-- Vypište všechny údaje osoby a setřiďte je podle id_osoba.
-- Hlasování PRO se pozná tak, že v hlasovani_poslanec.vysledek = 'A'.
select *
from osoba
where not exists (
	select 1
	from hlasovani
	join hlasovani_poslanec hp on hlasovani.id_hlasovani = hp.id_hlasovani
	join poslanec on poslanec.id_poslanec = hp.id_poslanec
	join organ on hlasovani.id_organ = organ.id_organ
	where nazev_dlouhy LIKE 'Návrh na vyslovení nedůvěry vládě%' and
		 year(organ.od_organ) = 2017 and
		 hp.vysledek = 'A' and
		 poslanec.id_osoba = osoba.id_osoba
) and exists (
	select 1
	from hlasovani
	join hlasovani_poslanec hp on hlasovani.id_hlasovani = hp.id_hlasovani
	join poslanec on poslanec.id_poslanec = hp.id_poslanec
	join organ on hlasovani.id_organ = organ.id_organ
	where nazev_dlouhy LIKE 'Návrh na vyslovení nedůvěry vládě%' and
		 year(organ.od_organ) = 2017 and
		 poslanec.id_osoba = osoba.id_osoba
)
order by id_osoba


-- B

-------------
-- 1. Nalezněte volební období, ve kterém byly v Poslanecké sněmovně alespoň dva kluby s minimálně 50 poslanci
-- (stačí počet záznamů v tabulce poslanec pro dané období).
-- Vypište id volebního období (id_organ), kdy volební období začínalo a počet klubů s více než 50 poslanci.
-- Poslanecký klub je záznam v organ pro který typ_organu.nazev_typ_org_cz = 'klub'.
SELECT t.id_organ, t.od_organ, count(*)
FROM (
	SELECT psp2013.id_organ, org.zkratka, org.nazev_organu_cz, psp2013.od_organ, count(*) cnt
	FROM poslanec p
	JOIN zarazeni z ON p.id_osoba = z.id_osoba and z.cl_funkce = 0
	JOIN organ org ON z.id_of = org.id_organ
	JOIN typ_organu typo ON org.id_typ_org = typo.id_typ_org
	JOIN organ psp2013 ON psp2013.id_organ = p.id_organ and org.rodic_id_organ = psp2013.id_organ
	WHERE typo.nazev_typ_org_cz = 'klub'
	GROUP BY org.zkratka, psp2013.od_organ, org.nazev_organu_cz, psp2013.id_organ
	HAVING count(*) >= 50
) t
GROUP BY t.id_organ, t.od_organ
HAVING count(*) >= 2



------
-- 2. Pro každou schůzi ve volebním období 2002 - 2006 nalezněte poslední hlasování.
-- K porovnání pořadí můžete využít atribut hlasovani.cislo.
-- Tzn. zajímá nás hlasování s nejvyšší hodnotou atributu hlasovani.cislo v rámci jedné schůze.
-- Setřiďte výsledek podle atributu schuze.schuze a vypište atributy id_hlasovani, schuze, cislo a schuze.nazev_dlouhy.
with tab as (
	select hlasovani.*
	from schuze
	join hlasovani on schuze.schuze = hlasovani.schuze and
						schuze.id_organ = hlasovani.id_organ
	join organ on organ.id_organ = schuze.id_organ
	where year(organ.od_organ) = 2002
)
select id_hlasovani, schuze, cislo, nazev_dlouhy
from tab t1
where t1.cislo >= all (
  select cislo
  from tab t2
  where t1.schuze = t2.schuze
)
order by t1.schuze



select hlasovani.*
from schuze
join hlasovani on schuze.schuze = hlasovani.schuze and
					schuze.id_organ = hlasovani.id_organ
join organ on organ.id_organ = schuze.id_organ
where year(organ.od_organ) = 2002 and schuze.schuze = 1


------
-- 3. Nalezněte osoby, které nikdy ve volebním období 2017 - 2021 nehlasovaly PRO u hlasování,
-- jehož dlouhý název (tj. hlasovani.nazev_dlouhy) obsahuje "COVID".
-- Vyberte pouze osoby, které hlasovaly v nějakém takovém hlasování, ale nikdy nehlasovaly PRO.
-- Vypište všechny údaje osoby a setřiďte je podle id_osoba.
-- Hlasování PRO se pozná tak, že v hlasovani_poslanec.vysledek = 'A'.
select *
from osoba
where not exists (
	select 1
	from hlasovani
	join hlasovani_poslanec hp on hlasovani.id_hlasovani = hp.id_hlasovani
	join poslanec on poslanec.id_poslanec = hp.id_poslanec
	join organ on hlasovani.id_organ = organ.id_organ
	where nazev_dlouhy LIKE '%COVID%' and
		 year(organ.od_organ) = 2017 and
		 hp.vysledek = 'A' and
		 poslanec.id_osoba = osoba.id_osoba
) and exists (
	select 1
	from hlasovani
	join hlasovani_poslanec hp on hlasovani.id_hlasovani = hp.id_hlasovani
	join poslanec on poslanec.id_poslanec = hp.id_poslanec
	join organ on hlasovani.id_organ = organ.id_organ
	where nazev_dlouhy LIKE '%COVID%' and
		 year(organ.od_organ) = 2017 and
		 poslanec.id_osoba = osoba.id_osoba
)


----------------------------------------------------
----------------------------------------------------
-- Středa 9:00

-- A


-- A

-- Příklad 1: Vypište osoby, které byly jmenovány do Vlády ČR
-- (organ.nazev_organu_cz like 'Vláda České republiky') v období Poslanecké sněmovny
-- (organ.nazev_organu_cz like 'Poslanecká sněmovna'), aniž by získaly mandát ve
-- volbách v tomto období poslanecké sněmovny.
-- Pro každou osobu vypište:
--   id_osoba, jméno a příjmení, počet takových jmenování, počet měsíců ve vládě
--   pro taková jmenování.
-- Setřiďte dle počtu takových jmenování. Uvažujte pouze osoby, které
-- byly takto jmenovány nejméně 3x.

select o1.id_osoba, o1.jmeno, o1.prijmeni, count(*) as pocet_jmenovani,
        sum(datediff(month, z1.od_o, z1.do_o)) as pocet_mesicu_ve_vlade from osoba o1
join zarazeni z1 on o1.id_osoba=z1.id_osoba
join organ org1 on z1.id_of=id_organ and z1.cl_funkce=0
where org1.nazev_organu_cz like 'Vláda České republiky'
and not exists
(
  select 1 from osoba o2
  join zarazeni z2 on o2.id_osoba=z2.id_osoba
  join organ org2 on z2.id_of=org2.id_organ and z2.cl_funkce=0
  where org2.nazev_organu_cz like 'Poslanecká sněmovna' and
	    (o1.id_osoba = o2.id_osoba and z1.od_o between org2.od_organ and org2.do_organ)
)
group by o1.id_osoba,o1.jmeno,o1.prijmeni
having count(*) > 2
order by pocet_jmenovani desc

/*
Výsledek: 2 záznamy
id_osoba	jmeno	prijmeni	pocet_jmenovani	pocet_mesicu_ve_vlade
5326	Milan	Šimonovský	3	49
5524	Alexandr	Vondra	3	61
*/

---------------------------------------------------------------------------------------

-- Příklad 2: Vypište osoby (id_osoba, jméno a příjmení), které se častěji omlouvaly v lednu
-- než v únoru a zároveň počet omluv v lednu > 33.
-- Setřiďte sestupně dle počtu omluv v lednu.

select * from
(
  select os.id_osoba,os.jmeno,os.prijmeni,
    count(case when month(om.den)=1 then 1 end) as pocet_dnu_leden,
    count(case when month(om.den)=2 then 1 end) as pocet_dnu_unor
  from osoba os
  join poslanec p on os.id_osoba=p.id_osoba
  join omluva om on om.id_poslanec=p.id_poslanec
  group by os.id_osoba,os.jmeno,os.prijmeni
) t
where t.pocet_dnu_leden > 33 and t.pocet_dnu_leden > t.pocet_dnu_unor
order by t.pocet_dnu_leden desc

/*
Výsledek: 7 záznamů
id_osoba	jmeno	prijmeni	pocet_dnu_leden	pocet_dnu_unor
6168	Helena	Válková	55	36
6150	Andrej	Babiš	54	31
6138	Pavel	Bělobrádek	44	35
6505	Julius	Špičák	39	17
401	Miroslava	Němcová	36	12
6166	Jan	Volný	35	33
6200	Milan	Chovanec	35	21
*/

---------------------------------------------------------------------------------------

-- Příklad 3: Vypište osoby, které byly místopředsedou sněmovny (typ_funkce.typ_funkce_cz='Místopředseda',
-- typ_organu.nazev_typ_org_cz='Parlament') více než jednou. Pro každou osobu uveďte:
--   id_osoba, jméno, příjmení, počet těchto funkcí, počet členství v delegacích
--   (typ_organu.nazev_typ_org_cz='Delegace').
-- Setřiďte sestupně dle počtu místopředsednické funkce.

select o.id_osoba, o.jmeno, o.prijmeni, count(*) as pocet,
  (
    select count(*) from osoba o2
    join zarazeni z2 on o2.id_osoba=z2.id_osoba
    join organ org2 on z2.id_of=org2.id_organ and z2.cl_funkce=0
	join typ_organu tor2 on org2.id_typ_org=tor2.id_typ_org
	where tor2.nazev_typ_org_cz='Delegace' and o2.id_osoba=o.id_osoba
  ) as pocet_clenstvi_delegace
from osoba o
join zarazeni z on o.id_osoba=z.id_osoba
join funkce f on z.id_of=f.id_funkce and z.cl_funkce=1
join typ_funkce tf on f.id_typ_funkce=tf.id_typ_funkce
join typ_organu tor on tor.id_typ_org=tf.id_typ_org
where tf.typ_funkce_cz='Místopředseda' and tor.nazev_typ_org_cz='Parlament'
group by o.id_osoba, o.jmeno, o.prijmeni
having count(*) > 1
order by pocet desc

-- Výsledek: 10 záznamů
id_osoba	jmeno	prijmeni	pocet	pocet_clenstvi_delegace
64	Jan	Kasal	4	3
303	Vojtěch	Filip	4	1
401	Miroslava	Němcová	2	5
5462	Jan	Hamáček	2	4
89	Karel	Ledvinka	2	0
6178	Jan	Bartošek	2	0
16	Petra	Buzková	2	0
181	Jiří	Vlach	2	1
252	Lubomír	Zaorálek	2	1
266	Ivan	Langer	2	1

-- B

-- Příklad 1: Vypište osoby, které byly v nějakém výboru poslanecké sněmovny
-- (typ_organ.nazev_typ_org_cz='Výbor') a zároveň ve stejném volebním období
-- (tj. orgány mají stejnou hodnotu organ.rodic_id_organ) nebyly
-- v žádné delegaci poslanecké sněmovny (typ_organ.nazev_typ_org_cz='Delegace').
-- Pro každou osobu vypište:
--   id_osoba, jméno a příjmení osoby, počet takových členství ve výborech,
--   počet všech členství v delegacích (pokud nebyl poslanec členem žádné delegace,
--   vypište hodnotu 0).
-- Setřiďte dle počtu členství ve všech delegacích.
-- Uvažujte pouze osoby, které byly členy takových výborů 8x.

select o1.id_osoba, o1.jmeno, o1.prijmeni, count(*) as pocet_vybor,
  coalesce(
  (
    select count(*) from osoba o2
    join zarazeni z2 on o2.id_osoba=z2.id_osoba
    join organ org2 on z2.id_of=id_organ and z2.cl_funkce=0
    join typ_organu tor2 on org2.id_typ_org=tor2.id_typ_org
    where tor2.nazev_typ_org_cz='Delegace' and o2.id_osoba=o1.id_osoba
  ), 0) as pocet_delegace
from osoba o1
join zarazeni z1 on o1.id_osoba=z1.id_osoba
join organ org1 on z1.id_of=id_organ and z1.cl_funkce=0
join typ_organu tor1 on org1.id_typ_org=tor1.id_typ_org
where tor1.nazev_typ_org_cz='Výbor' and
not exists (
  select 1 from osoba o2
  join zarazeni z2 on o2.id_osoba=z2.id_osoba
  join organ org2 on z2.id_of=id_organ and z2.cl_funkce=0
  join typ_organu tor2 on org2.id_typ_org=tor2.id_typ_org
  where tor2.nazev_typ_org_cz='Delegace' and o2.id_osoba=o1.id_osoba
        and org1.rodic_id_organ=org2.rodic_id_organ
)
group by o1.id_osoba, o1.jmeno, o1.prijmeni
having count(*) = 8
order by pocet_delegace desc

-- Příklad 2: Vypište osoby (id_osoba, jméno, příjmení) aktuální sněmovny (rok organ.od_organ je 2021),
-- které se častěji omlouvaly v dubnu než v květnu a zároveň počet omluv v dubnu > 11.
-- Setřiďte sestupně dle počtu omluv v dubnu.

select * from
(
  select os.id_osoba,os.jmeno,os.prijmeni,
    count(case when month(om.den)=4 then 1 end) as pocet_dnu_duben,
    count(case when month(om.den)=5 then 1 end) as pocet_dnu_kveten
  from osoba os
  join poslanec p on os.id_osoba=p.id_osoba
  join omluva om on om.id_poslanec=p.id_poslanec
  join organ org on om.id_organ=org.id_organ
  where year(org.od_organ)=2021
  group by os.id_osoba,os.jmeno,os.prijmeni
) t
where t.pocet_dnu_duben > 11 and t.pocet_dnu_duben > t.pocet_dnu_kveten
order by t.pocet_dnu_duben desc

-- Příklad 3: Vypište osoby, které byly členy Ústavně právního výboru
-- (organ.nazev_organu_cz je 'Ústavně právní výbor' nebo 'Ústavně-právní výbor')
-- více než 3x. Pro každou osobu vypište:
--   id_osoba, jméno, příjmení, počet členství ve výboru, celkový počet členství
--   v jakémkoli výboru  (typ_organu.nazev_typ_org_cz='Výbor').
-- Setřiďte sestupně dle počtu v Ústavně právním výboru.

select o.id_osoba, o.jmeno, o.prijmeni, count(*) as pocet_up_vybor,
  (
    select count(*) from osoba o2
    join zarazeni z2 on o2.id_osoba=z2.id_osoba
    join organ org2 on z2.id_of=org2.id_organ and z2.cl_funkce=0
    join typ_organu tor2 on org2.id_typ_org=tor2.id_typ_org
    where tor2.nazev_typ_org_cz='Výbor' and o2.id_osoba=o.id_osoba
  ) as pocet_clenstvi_ve_vyborech
from osoba o
join zarazeni z on o.id_osoba=z.id_osoba
join organ org on z.id_of=org.id_organ and z.cl_funkce=0
where org.nazev_organu_cz='Ústavně právní výbor' or org.nazev_organu_cz='Ústavně-právní výbor'
group by o.id_osoba, o.jmeno, o.prijmeni
having count(*) > 3
order by pocet_up_vybor desc

/*
Výsledek: 5 záznamů
id_osoba  jmeno   prijmeni  pocet_up_vybor  pocet_clenstvi_ve_vyborech
4         Marek   Benda    9               27
218       Zdeněk  Jičínský 4               6
386       Stanislav Křeček  4               12
4778      Stanislav Grospič 4               7
6168      Helena  Válková  4               11
*/


----------------------------------------------------
----------------------------------------------------
-- Středa 10:45

-- A

-- Příklad 1: Vypište osoby, které byly ve stejném volebním období poslanecké sněmovny
-- členy poslaneckého klubu Unie svobody (organ.nazev_organu_cz='Poslanecký klub Unie svobody')
-- a ve stejném volebním období (tj. orgány poslaneckých klubů mají stejnou hodnotu organ.rodic_id_organ)
-- nebyly členy poslaneckého klubu Občanské demokratické strany
-- (organ.nazev_organu_cz='Poslanecký klub Občanské demokratické strany').
-- Pro každou osobu vypište:
-- id_osoba, jméno, příjmení, počet měsíců poslaneckého mandátu odpovídajících výše popsané podmínce,
-- celkový počet měsíců v poslanecké sněmovně.
-- Setřiďte dle příjmení.

select o1.id_osoba, o1.jmeno, o1.prijmeni,
  sum(datediff(month, z1.od_o, z1.do_o)) as pocet_mesicu_us,
  (
    select sum(datediff(month, z2.od_o, z2.do_o)) as pocet_mesicu_ods from osoba o2
    join zarazeni z2 on o2.id_osoba=z2.id_osoba
    join organ org2 on z2.id_of=org2.id_organ and z2.cl_funkce=0
    where org2.nazev_organu_cz='Poslanecká sněmovna' and o2.id_osoba=o1.id_osoba
	group by o2.id_osoba
  ) as pocet_mesicu_poslanec
from osoba o1
join zarazeni z1 on o1.id_osoba=z1.id_osoba
join organ org1 on z1.id_of=org1.id_organ and z1.cl_funkce=0
where org1.nazev_organu_cz='Poslanecký klub Unie svobody'
and not exists (
  select o3.id_osoba, o3.jmeno, o3.prijmeni from osoba o3
  join zarazeni z3 on o3.id_osoba=z3.id_osoba
  join organ org3 on z3.id_of=org3.id_organ and z3.cl_funkce=0
  where org3.nazev_organu_cz='Poslanecký klub Občanské demokratické strany'
        and o3.id_osoba=o1.id_osoba and org3.rodic_id_organ=org1.rodic_id_organ
)
group by o1.id_osoba, o1.jmeno, o1.prijmeni
order by o1.prijmeni

/*
Výsledek: 22 záznamů
id_osoba  jmeno   prijmeni  pocet_mesicu_us  pocet_mesicu_poslanec
38        Andrej  Gjurič    4                53
364       Ivana   Hanačíková 48               48
380       Ladislav Korbel   48               48
381       Zdeněk  Kořistka  48               71
79        Václav  Krása     48               120
...
*/

---------------------------------------------------------------------------

-- Příklad 2: Vypište osoby poslaneckého klubu ODS
-- (organ.nazev_organu_cz='Poslanecký klub Občanské demokratické strany'),
-- které se častěji omlouvaly v listopadu než v prosinci a zároveň počet omluv v listopadu > 16.
-- Setřiďte sestupně dle počtu omluv v listopadu.

select * from
(
  select os.id_osoba, os.jmeno, os.prijmeni,
    count(case when month(om.den)=11 then 1 end) as pocet_dnu_listopad,
    count(case when month(om.den)=12 then 1 end) as pocet_dnu_prosinec
  from osoba os
  join poslanec p on os.id_osoba=p.id_osoba
  join omluva om on om.id_poslanec=p.id_poslanec
  where os.id_osoba in (
    select z.id_osoba
    from  zarazeni z
    join organ org on z.id_of=org.id_organ and z.cl_funkce=0
    where org.nazev_organu_cz='Poslanecký klub Občanské demokratické strany'
  )
  group by os.id_osoba, os.jmeno, os.prijmeni
) t
where t.pocet_dnu_listopad > 16 and t.pocet_dnu_listopad > t.pocet_dnu_prosinec
order by t.pocet_dnu_listopad desc

/*
Výsledek: 7 záznamů
id_osoba  jmeno                prijmeni              pocet_dnu_listopad  pocet_dnu_prosinec
6538      Martin               Kupka                 24                  21
6467      Zuzana               Majerová Zahradníková 19                  12
*/

-- Příklad 3: Vypište osoby, které byly více než šestkrát členy poslaneckého klubu
-- (typ_organu.nazev_typ_org_cz='Klub') se stejným názvem (tzn. se stejnou
-- hodnotou organ.nazev_organu_cz). Pro každou osobu vypište:
--   id_osoba, jméno, příjmení, název poslaneckého klubu, počet
--   členství v poslaneckém klubu, počet období sněmovny ve kterých byla osoba
--   poslancem.
-- Setřiďte sestupně dle počtu členství.

select o.id_osoba, o.jmeno, o.prijmeni, org.nazev_organu_cz,
  count(*) as pocet_klubu,
  (
    select count(*) from poslanec
	where poslanec.id_osoba=o.id_osoba
  ) as pocet_volebnich_obdobi
from osoba o
join zarazeni z on o.id_osoba=z.id_osoba
join organ org on z.id_of=org.id_organ and z.cl_funkce=0
join typ_organu tor on tor.id_typ_org=org.id_typ_org
where tor.nazev_typ_org_cz='Klub'
group by o.id_osoba, o.jmeno, o.prijmeni, org.nazev_organu_cz
having count(*) > 6
order by pocet_klubu desc

/*
Výsledek: 6 záznamů
id_osoba	jmeno	prijmeni	nazev_organu_cz	pocet_klubu	pocet_volebnich_obdobi
4	Marek	Benda	Poslanecký klub Občanské demokratické strany	9	9
237	Bohuslav	Sobotka	Poslanecký klub České strany sociálně demokratické	7	7
252	Lubomír	Zaorálek	Poslanecký klub České strany sociálně demokratické	7	7
303	Vojtěch	Filip	Poslanecký klub Komunistické strany Čech a Moravy	7	7
305	Miroslav	Grebeníček	Poslanecký klub Komunistické strany Čech a Moravy	7	7
309	Pavel	Kováčik	Poslanecký klub Komunistické strany Čech a Moravy	7	7
*/

-- B

-- Příklad 1: Vypište osoby, které byly/jsou členy poslaneckého klubu TOP 09
-- (organ.nazev_organu_cz='Poslanecký klub TOP 09')
-- a nebyly členy poslaneckého klubu Občanské demokratické strany
-- (organ.nazev_organu_cz='Poslanecký klub Občanské demokratické strany')
-- ve stejném volebním období (tj. orgány poslaneckých klubů mají stejnou hodnotu
-- organ.rodic_id_organ).
-- Pro každou osobu vypište:
--   id_osoba, jméno, příjmení, počet měsíců poslaneckého mandátu v TOP 09 (v případě
--   poslaneckého mandátu jen v aktuálním nedokončeném období, vypište počet měsíců
--   od začátku mandátu k aktuálnímu datu).
-- Setřiďte dle příjmení.

select o1.id_osoba, o1.jmeno, o1.prijmeni,
  coalesce(sum(datediff(month, z1.od_o, z1.do_o)),
           sum(datediff(month, z1.od_o, GetDate()))) as pocet_mesicu_top09
from osoba o1
join zarazeni z1 on o1.id_osoba=z1.id_osoba
join organ org1 on z1.id_of=org1.id_organ and z1.cl_funkce=0
where org1.nazev_organu_cz='Poslanecký klub TOP 09'
and not exists (
  select 1 from osoba o2
  join zarazeni z2 on o2.id_osoba=z2.id_osoba
  join organ org2 on z2.id_of=org2.id_organ and z2.cl_funkce=0
  where o2.id_osoba=o1.id_osoba and
        org2.nazev_organu_cz='Poslanecký klub Občanské demokratické strany' and
        org2.rodic_id_organ=org1.rodic_id_organ
)
group by o1.id_osoba, o1.jmeno, o1.prijmeni
order by pocet_mesicu_top09 desc

/*
Výsledek: 19 záznamů
id_osoba	jmeno	prijmeni	pocet_mesicu_top09
5558	Karel	Schwarzenberg	48
5943	Helena	Langšádlová	48
6137	František	Vácha	48
6147	Markéta	Pekarová Adamová	48
...
6802	Pavel	Svoboda	24
6687	Jan	Jakob	9
6707	Olga	Sommerová	5
*/

-- Příklad 2: Vypište osoby, které se častěji omlouvaly v první polovině roku 2021 než ve druhé
-- polovině roku 2021 a zároveň počet omluv v první polovině roku > 47.
-- Setřiďte sestupně dle počtu omluv v první polovině roku.

select * from
(
  select os.id_osoba,os.jmeno,os.prijmeni,
    count(case when month(om.den) >= 1 and month(om.den) <= 6 then 1 end) as pocet_dnu_1_6,
    count(case when month(om.den) >= 7 and month(om.den) <= 12 then 1 end) as pocet_dnu_7_12
  from osoba os
  join poslanec p on os.id_osoba=p.id_osoba
  join omluva om on om.id_poslanec=p.id_poslanec
  where year(om.den) = 2021
  group by os.id_osoba,os.jmeno,os.prijmeni
  having count(case when month(om.den) >= 1 and month(om.den) <= 6 then 1 end) > 47
) t
where t.pocet_dnu_1_6 > 47 and t.pocet_dnu_1_6 > t.pocet_dnu_7_12
order by t.pocet_dnu_1_6 desc

/*
Výsledek: 8 záznamů
id_osoba	jmeno	prijmeni	pocet_dnu_1_6	pocet_dnu_7_12
6465	Lenka	Kozlová	56	13
5558	Karel	Schwarzenberg	54	14
6502	Jana	Levová	54	17
5462	Jan	Hamáček	51	18
6147	Markéta	Pekarová Adamová	50	10
6150	Andrej	Babiš	50	18
6074	Petr	Fiala	48	8
6467	Zuzana	Majerová Zahradníková	48	15
*/

-- Příklad 3: Vypište osoby, které v aktuálním volebním období změnily poslanecký klub
-- (typ_organu.nazev_typ_org_cz='Klub'), tedy jsou/byly členy více než jednoho poslaneckého klubu.
-- Pro každou osobu vypište:
--   id_osoba, jméno, příjmení, počet poslaneckých klubů v aktuálním volebním období,
--   počet měsíců v poslaneckých klubech s ukončeným členstvím v aktuálním volebním období.
-- Setřiďte sestupně dle počtu klubů.
--
-- Poznámka: organ.od_organ poslaneckého klubu musí být mezi organ.od_organ a organ.do_organ
-- orgánu období poslanecké sněmovny.

select o.id_osoba, o.jmeno, o.prijmeni,
  count(distinct org.nazev_organu_cz) as pocet_ruznych_klubu,
  sum(datediff(month, z.od_o, do_o)) pocet_mesicu_ukoncenych_clenstvi
from osoba o
join zarazeni z on o.id_osoba=z.id_osoba
join organ org on z.id_of=org.id_organ and z.cl_funkce=0
join typ_organu tor on tor.id_typ_org=org.id_typ_org
join poslanec p on p.id_osoba=o.id_osoba
join organ snemovna on p.id_organ=snemovna.id_organ
where tor.nazev_typ_org_cz='Klub' and year(snemovna.od_organ)=2021 and
      org.od_organ >= snemovna.od_organ
group by o.id_osoba, o.jmeno, o.prijmeni
having count(distinct org.nazev_organu_cz) > 1
order by pocet_ruznych_klubu desc

/*
Výsledek: 1 záznam
id_osoba	jmeno	prijmeni	pocet_ruznych_klubu	pocet_mesicu_ukoncenych_clenstvi
6492	Ivo	Vondrák	2	16
*/


---

SELECT os.jmeno, COUNT(*) pocet
FROM poslanec p
JOIN osoba os ON os.id_osoba = p.id_osoba
JOIN zarazeni z ON z.id_osoba = os.id_osoba
JOIN organ org ON org.id_organ = z.id_of AND cl_funkce = 0
JOIN organ org2 ON org2.id_organ = p.id_organ AND org2.id_organ = org.rodic_id_organ
GROUP BY os.jmeno
ORDER BY pocet DESC

SELECT os.jmeno, COUNT(*) pocet
FROM osoba os
WHERE EXISTS (
    SELECT 1
    FROM poslanec p
    JOIN zarazeni z ON p.id_osoba = z.id_osoba
    JOIN organ org ON org.id_organ = z.id_of AND z.cl_funkce = 0
    JOIN organ org2 ON org2.id_organ = p.id_organ AND org2.id_organ = org.rodic_id_organ
    WHERE os.id_osoba = p.id_osoba
)
GROUP BY os.jmeno
ORDER BY pocet DESC


WITH ctx AS (SELECT os.id_osoba, os.jmeno, os.prijmeni, p.id_kraj
             FROM poslanec p
                      JOIN osoba os ON os.id_osoba = p.id_osoba
                      JOIN zarazeni z ON z.id_osoba = os.id_osoba
                      JOIN organ org ON org.id_organ = z.id_of AND z.cl_funkce = 0
                      JOIN organ org2 ON org2.id_organ = p.id_organ AND org2.id_organ = org.rodic_id_organ
             WHERE YEAR(org.od_organ) >= 2013)

SELECT t1.*
FROM ctx t1
WHERE EXISTS (
    SELECT 1
    FROM ctx t2
    WHERE t2.id_osoba = t1.id_osoba AND
          t2.id_kraj = 594
    ) AND E
