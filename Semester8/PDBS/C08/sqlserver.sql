set statistics time on;
set statistics io on;

SELECT * FROM Customer
WHERE residence = 'Ostrava'
ORDER BY lname,idc
offset 0 rows
fetch next 100 rows only;

/*
SELECT * BEZ fetch next
[2026-04-14 09:15:19] 15,183 rows retrieved starting from 1 in 1 s 35 ms (execution: 186 ms, fetching: 849 ms)
fetch next 100 rows only;
[2026-04-14 09:15:26] 100 rows retrieved starting from 1 in 785 ms (execution: 366 ms, fetching: 419 ms)
*/

create table OrderItem_ct (
                           ido int references "Order"(ido) not null,
                           idp int references Product(idp) not null,
                           unit_price bigint not null,
                           quantity int not null
);

insert into OrderItem_ct SELECT * FROM OrderItem;

create clustered index idx_order_item on dbo.OrderItem_ct(ido, idp);

exec PrintIndexes 'OrderItem_ct';
exec PrintPagesIndex 'idx_order_item';

SELECT * FROM OrderItem_ct
WHERE unit_price <= 311;

alter table OrderItem_ct rebuild partition = all
with ( data_compression = none);

alter table OrderItem_ct rebuild partition = all
    with ( data_compression = row);

alter table OrderItem_ct rebuild partition = all
    with ( data_compression = page);

exec PrintPagesHeap 'OrderItem';



create clustered columnstore index idx_orderitem_col
    on OrderItem;

exec PrintIndexes 'OrderItem';
exec PrintPagesIndex 'idx_orderitem_col';

drop index idx_orderitem_col on OrderItem;

set statistics time on;
set statistics io on;
SET SHOWPLAN_ALL OFF;

select avg(UNIT_PRICE) FROM ORDERITEM option (maxdop 1);

select avg(UNIT_PRICE), QUANTITY FROM ORDERITEM
GROUP BY QUANTITY option (maxdop 1);