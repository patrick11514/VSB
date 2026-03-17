with tab as (
    select lname, fname, residence, count(*) c
    from Customer
    group by lname, fname, residence
)
select *
from tab
where c >= all (select c from tab)
union all
select *
from tab
where c <= all (select c from tab);

explain plan for
select *
from Customer
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select * from Customer where lname = %';

begin
   PrintQueryStat('byrrkp5uv9ypg', 169474562);
end;

create index idx_customer_ln_fn_rs on Customer(lname, fname, residence);


select *
from Customer
where lname = 'Weber' AND fname = 'Hana' AND residence = 'Berlin';
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select * from Customer where lname = %';

begin
    PrintQueryStat('dkz2fphx55sp1', 169474562);
end;

with tab as (
    select lname, fname, count(*) c
    from Customer
    group by lname, fname
)
select *
from tab
where c >= all (select c from tab)
union all
select *
from tab
where c <= all (select c from tab);

explain plan for
select lname, fname
from Customer
where lname = 'Nováková' AND fname = 'Alena';
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select lname, fname from Customer where lname = %';

begin
    PrintQueryStat('7xn72p13083px', 4070000281);
end;


with tab as (
    select lname, residence, count(*) c
    from Customer
    group by lname, residence
)
select *
from tab
where c >= all (select c from tab)
union all
select *
from tab
where c <= all (select c from tab);



explain plan for
select lname, residence
from Customer
where lname = 'Nováková' AND residence = 'Praha';
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select lname, residence from Customer where lname = %';

begin
    PrintQueryStat('7j5m6q3dy3394', 2040120807);
end;