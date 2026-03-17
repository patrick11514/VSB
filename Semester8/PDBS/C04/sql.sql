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

SET SHOWPLAN_ALL ON;

select *
from Customer 
where lname = 'Svoboda' AND fname = 'Pavel' AND residence = 'Barcelona'
option (maxdop 1);

create index idx_customer_ln_fn_rs on Customer(lname, fname, residence);

SET SHOWPLAN_ALL OFF;

set statistics io on;
set statistics time on;

set statistics io off;
set statistics time off;

select *
from Customer
where lname = 'Nováková' AND fname = 'Jana' AND residence = 'Beroun'
option (maxdop 1);

select count(*) from dbo.Customer;

select *
from Customer
where lname = 'Jones' AND fname = 'Milan'
option (maxdop 1);

select lname, fname
from Customer
where lname = 'Jones' AND fname = 'Milan'
option (maxdop 1);

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

select fname, residence
from Customer
where fname = 'Nováková' AND residence = 'Bratislava'
option (maxdop 1);

exec PrintPagesHeap 'Customer';
exec PrintIndexes 'Customer';
exec PrintPagesIndex 'PK__Customer__DC501A0C5BE68E27';
exec PrintPagesIndex  'idx_customer_ln_fn_rs';