CREATE TABLE Volebniobdobi (
    id_obdobi INT IDENTITY,
    zacatek DATE NOT NULL,
    konec DATE DEFAULT NULL,
    PRIMARY KEY (id_obdobi)
)

ALTER TABLE Volebniobdobi
ADD PRIMARY KEY (id_obdobi)

INSERT INTO Volebniobdobi(zacatek, konec)
SELECT organ.od_organ as zacatek, organ.do_organ as konec
FROM organ
WHERE organ.id_organ BETWEEN 165 AND 173

SELECT * FROM Volebniobdobi

ALTER TABLE omluva
DROP CONSTRAINT omluvy_fk_id_organ

UPDATE omluva SET id_organ = (
    select id_obdobi from Volebniobdobi
    join organ o on o.id_organ = omluva.id_organ
    where o.od_organ = Volebniobdobi.zacatek
)


ALTER TABLE omluva
ADD CONSTRAINT fk_organ_obdobi FOREIGN KEY (id_organ) references Volebniobdobi(id_obdobi)



create table VolebniObdobi
(
    id_obdobi int identity primary key not null,
    zacatek date not null,
    konec date null default null,
)

select *from VolebniObdobi
select *from poslanec

--2
insert into VolebniObdobi
    select od_organ, do_organ from organ
    where id_organ >= 165 and id_organ <= 173 

--3
alter table poslanec
drop constraint poslanec_fk_id_obdobi

--4
update poslanec
set id_organ =
(
    select id_obdobi from VolebniObdobi
    join organ org on org.od_organ = VolebniObdobi.zacatek
    join poslanec p on p.id_organ = org.id_organ
    where p.id_poslanec = poslanec.id_poslanec
)

--5
alter table poslanec
add constraint fk_poslanec_id_org foreign key (id_organ)
    references VolebniObdobi (id_obdobi)

select * from poslanec
join VolebniObdobi on VolebniObdobi.id_obdobi = poslanec.id_organ
select * from VolebniObdobi