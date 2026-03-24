create table Customer_ct (
                          idc int primary key nonclustered,
                          fname varchar(20) not null,
                          lname varchar(30) not null,
                          residence varchar(20) not null,
                          gender char(1) not null,
                          birthday date not null
);
    
INSERT INTO Customer_ct SELECT * FROM dbo.Customer;

create or alter procedure PrintPagesClusteredTable
@tableName varchar(30)
as
    exec PrintPages @tableName, 1


create clustered index Customer_ct on Customer_ct(idc);

with tab as (
    select lname, fname, residence, count(*) c
    from Customer_ct
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
    SET SHOWPLAN_ALL OFF;
    
select *
from Customer_ct
where lname = 'Svoboda' AND fname = 'Pavel' AND residence = 'Barcelona'
option (maxdop 1);


    set statistics io on;
    set statistics time on;
    set statistics io off;
    set statistics time off;