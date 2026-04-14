select index_name from user_indexes
where table_name='OrderItem';

select blocks, bytes/1024/1024 as MB from user_segments
where segment_name = 'ORDERITEM';

SELECT * FROM ORDERITEM;

explain plan for
select avg(UNIT_PRICE) FROM ORDERITEM;
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select avg(UNIT_PRICE) FROM%';


begin
    PrintQueryStat('383t2tr24pkp3', 1670445072);
end;

explain plan for
select avg(UNIT_PRICE), QUANTITY FROM ORDERITEM
    GROUP BY QUANTITY;
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select avg(UNIT_PRICE), QUANTITY FROM%';


begin
    PrintQueryStat('fd83dnkt8f9cd', 2014048680);
end;

alter table ORDERITEM inmemory;

