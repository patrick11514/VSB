select * from
    (
        SELECT qs.execution_count,
               SUBSTRING(qt.text,qs.statement_start_offset/2 +1,
                         (CASE WHEN qs.statement_end_offset = -1
                                   THEN LEN(CONVERT(nvarchar(max), qt.text)) * 2
                               ELSE qs.statement_end_offset end -
                          qs.statement_start_offset
                             )/2
               ) AS query_text,
               qs.total_worker_time/qs.execution_count AS avg_cpu_time, qp.dbid
        --, qt.text, qs.plan_handle, qp.query_plan   
        FROM sys.dm_exec_query_stats AS qs
                 CROSS APPLY sys.dm_exec_query_plan(qs.plan_handle) as qp
                 CROSS APPLY sys.dm_exec_sql_text(qs.sql_handle) as qt
        where qp.dbid=DB_ID() and qs.execution_count > 10
    ) t
where query_text like 'SELECT count(*)%'
order by avg_cpu_time desc;

SET SHOWPLAN_ALL ON;
SET SHOWPLAN_ALL OFF;

set statistics io on;
set statistics time on;
set statistics io off;
set statistics time off;

--1)
-- start
    /*
"SELECT * FROM Product p
INNER JOIN OrderItem oi ON oi.idp = p.idp
WHERE p.unit_price between 20000000 and 20002000
option (maxdop 1)",1,1,0,,,1,,104.866486,,,,19.358751,,,SELECT,false,
"  |--Hash Match(Inner Join, HASH:([p].[idp])=([oi].[idp])DEFINE:([Opt_Bitmap1005]))",1,2,1,Hash Match,Inner Join,HASH:([p].[idp])=([oi].[idp]),[Opt_Bitmap1005],104.866486,0,0.00955518,1074,19.358751,"[p].[idp], [p].[name], [p].[unit_price], [p].[producer], [p].[description], [oi].[ido], [oi].[idp], [oi].[unit_price], [oi].[quantity]",,PLAN_ROW,false,1
"       |--Table Scan(OBJECT:([MIN0150].[dbo].[Product] AS [p]),  WHERE:([MIN0150].[dbo].[Product].[unit_price] as [p].[unit_price]>=(20000000) AND [MIN0150].[dbo].[Product].[unit_price] as [p].[unit_price]<=(20002000)))",1,3,2,Table Scan,Table Scan,"OBJECT:([MIN0150].[dbo].[Product] AS [p]),  WHERE:([MIN0150].[dbo].[Product].[unit_price] as [p].[unit_price]>=(20000000) AND [MIN0150].[dbo].[Product].[unit_price] as [p].[unit_price]<=(20002000))","[p].[idp], [p].[name], [p].[unit_price], [p].[producer], [p].[description]",2.0973296,0.37793982,0.110157,1053,0.4880968,"[p].[idp], [p].[name], [p].[unit_price], [p].[producer], [p].[description]",,PLAN_ROW,false,1
"       |--Table Scan(OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]),  WHERE:(PROBE([Opt_Bitmap1005],[MIN0150].[dbo].[OrderItem].[idp] as [oi].[idp])))",1,5,2,Table Scan,Table Scan,"OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]),  WHERE:(PROBE([Opt_Bitmap1005],[MIN0150].[dbo].[OrderItem].[idp] as [oi].[idp]))","[oi].[ido], [oi].[idp], [oi].[unit_price], [oi].[quantity]",50,13.27794,5.495157,27,18.773096,"[oi].[ido], [oi].[idp], [oi].[unit_price], [oi].[quantity]",,PLAN_ROW,false,1

    */
-- cpu time: 422ms
-- logical reads: 17922
SELECT * FROM Product p
INNER JOIN OrderItem oi ON oi.idp = p.idp
WHERE p.unit_price between 20000000 and 20002000
option (maxdop 1);

-- 0.002 %
-- vysoká selektivita -> vytvořím index na fk
SELECT 
    cast(sum(case when p.unit_price between 20000000 and 20002000 then 1 end) as float) / count(*) * 100 
from Product p;

-- it 1
    /*
"SELECT * FROM Product p
INNER JOIN OrderItem oi ON oi.idp = p.idp
WHERE p.unit_price between 20000000 and 20002000
option (maxdop 1)",1,1,0,,,1,,104.86648,,,,0.92252004,,,SELECT,false,
"  |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1002], [Expr1006]) WITH UNORDERED PREFETCH)",1,2,1,Nested Loops,Inner Join,"OUTER REFERENCES:([Bmk1002], [Expr1006]) WITH UNORDERED PREFETCH",,104.86648,0,0.00043834187,1074,0.92252004,"[p].[idp], [p].[name], [p].[unit_price], [p].[producer], [p].[description], [oi].[ido], [oi].[idp], [oi].[unit_price], [oi].[quantity]",,PLAN_ROW,false,1
"       |--Nested Loops(Inner Join, OUTER REFERENCES:([p].[idp]))",1,4,2,Nested Loops,Inner Join,OUTER REFERENCES:([p].[idp]),,104.86648,0,0.00043834187,1065,0.5818493,"[p].[idp], [p].[name], [p].[unit_price], [p].[producer], [p].[description], [Bmk1002], [oi].[idp]",,PLAN_ROW,false,1
"       |    |--Table Scan(OBJECT:([MIN0150].[dbo].[Product] AS [p]), WHERE:([MIN0150].[dbo].[Product].[unit_price] as [p].[unit_price]>=(20000000) AND [MIN0150].[dbo].[Product].[unit_price] as [p].[unit_price]<=(20002000)))",1,5,4,Table Scan,Table Scan,"OBJECT:([MIN0150].[dbo].[Product] AS [p]), WHERE:([MIN0150].[dbo].[Product].[unit_price] as [p].[unit_price]>=(20000000) AND [MIN0150].[dbo].[Product].[unit_price] as [p].[unit_price]<=(20002000))","[p].[idp], [p].[name], [p].[unit_price], [p].[producer], [p].[description]",2.0973296,0.37793982,0.110157,1053,0.4880968,"[p].[idp], [p].[name], [p].[unit_price], [p].[producer], [p].[description]",,PLAN_ROW,false,1
"       |    |--Index Seek(OBJECT:([MIN0150].[dbo].[OrderItem].[idx_orderitem_idp] AS [oi]), SEEK:([oi].[idp]=[MIN0150].[dbo].[Product].[idp] as [p].[idp]) ORDERED FORWARD)",1,6,4,Index Seek,Index Seek,"OBJECT:([MIN0150].[dbo].[OrderItem].[idx_orderitem_idp] AS [oi]), SEEK:([oi].[idp]=[MIN0150].[dbo].[Product].[idp] as [p].[idp]) ORDERED FORWARD","[Bmk1002], [oi].[idp]",50,0.003125,0.000212,19,0.0053141485,"[Bmk1002], [oi].[idp]",,PLAN_ROW,false,2.0973296
"       |--RID Lookup(OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD)",1,8,2,RID Lookup,RID Lookup,"OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD","[oi].[ido], [oi].[unit_price], [oi].[quantity]",1,0.003125,0.0001581,23,0.3402324,"[oi].[ido], [oi].[unit_price], [oi].[quantity]",,PLAN_ROW,false,104.86648

    */
-- cpu time: 15ms
-- logical reads: 109+4
create index idx_orderitem_idp on dbo.OrderItem(idp);

-- it 2
    /*
"SELECT * FROM Product p
INNER JOIN OrderItem oi ON oi.idp = p.idp
WHERE p.unit_price between 20000000 and 20002000
option (maxdop 1)",1,1,0,,,1,,104.86648,,,,0.35491484,,,SELECT,false,
"  |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1002], [Expr1006]) WITH UNORDERED PREFETCH)",1,2,1,Nested Loops,Inner Join,"OUTER REFERENCES:([Bmk1002], [Expr1006]) WITH UNORDERED PREFETCH",,104.86648,0,0.00043834187,1074,0.35491484,"[p].[idp], [p].[name], [p].[unit_price], [p].[producer], [p].[description], [oi].[ido], [oi].[idp], [oi].[unit_price], [oi].[quantity]",,PLAN_ROW,false,1
"       |--Nested Loops(Inner Join, OUTER REFERENCES:([p].[idp]))",1,4,2,Nested Loops,Inner Join,OUTER REFERENCES:([p].[idp]),,104.86648,0,0.00043834187,1065,0.014244107,"[p].[idp], [p].[name], [p].[unit_price], [p].[producer], [p].[description], [Bmk1002], [oi].[idp]",,PLAN_ROW,false,1
"       |    |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1000]))",1,5,4,Nested Loops,Inner Join,OUTER REFERENCES:([Bmk1000]),,2.0973296,0,0.000008766838,1053,0.008491617,"[p].[idp], [p].[name], [p].[unit_price], [p].[producer], [p].[description]",,PLAN_ROW,false,1
"       |    |    |--Index Seek(OBJECT:([MIN0150].[dbo].[Product].[idx_product_unit_price] AS [p]), SEEK:([p].[unit_price] >= (20000000) AND [p].[unit_price] <= (20002000)) ORDERED FORWARD)",1,6,5,Index Seek,Index Seek,"OBJECT:([MIN0150].[dbo].[Product].[idx_product_unit_price] AS [p]), SEEK:([p].[unit_price] >= (20000000) AND [p].[unit_price] <= (20002000)) ORDERED FORWARD","[Bmk1000], [p].[unit_price]",2.0973296,0.003125,0.00015930706,19,0.003284307,"[Bmk1000], [p].[unit_price]",,PLAN_ROW,false,1
"       |    |    |--RID Lookup(OBJECT:([MIN0150].[dbo].[Product] AS [p]), SEEK:([Bmk1000]=[Bmk1000]) LOOKUP ORDERED FORWARD)",1,8,5,RID Lookup,RID Lookup,"OBJECT:([MIN0150].[dbo].[Product] AS [p]), SEEK:([Bmk1000]=[Bmk1000]) LOOKUP ORDERED FORWARD","[p].[idp], [p].[name], [p].[producer], [p].[description]",1,0.003125,0.0001581,1049,0.0051985425,"[p].[idp], [p].[name], [p].[producer], [p].[description]",,PLAN_ROW,false,2.0973296
"       |    |--Index Seek(OBJECT:([MIN0150].[dbo].[OrderItem].[idx_orderitem_idp] AS [oi]), SEEK:([oi].[idp]=[MIN0150].[dbo].[Product].[idp] as [p].[idp]) ORDERED FORWARD)",1,9,4,Index Seek,Index Seek,"OBJECT:([MIN0150].[dbo].[OrderItem].[idx_orderitem_idp] AS [oi]), SEEK:([oi].[idp]=[MIN0150].[dbo].[Product].[idp] as [p].[idp]) ORDERED FORWARD","[Bmk1002], [oi].[idp]",50,0.003125,0.000212,19,0.0053141485,"[Bmk1002], [oi].[idp]",,PLAN_ROW,false,2.0973296
"       |--RID Lookup(OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD)",1,11,2,RID Lookup,RID Lookup,"OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD","[oi].[ido], [oi].[unit_price], [oi].[quantity]",1,0.003125,0.0001581,23,0.3402324,"[oi].[ido], [oi].[unit_price], [oi].[quantity]",,PLAN_ROW,false,104.86648
*/
-- cpu time: 883us
-- logical reads: 109+4
create index idx_product_unit_price on dbo.Product(unit_price);

--2)
--"SELECT count(*), sum(oi.quantity) FROM Product p
--        INNER JOIN OrderItem oi ON oi.idp = p.idp
--WHERE p.unit_price between 20000000 and 20002000
--option (maxdop 1)"
--"  |--Compute Scalar(DEFINE:([Expr1004]=CONVERT_IMPLICIT(int,[Expr1015],0), [Expr1005]=CASE WHEN [Expr1015]=(0) THEN NULL ELSE [Expr1016] END))"
--"       |--Stream Aggregate(DEFINE:([Expr1015]=Count(*), [Expr1016]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])))"
--"            |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1002], [Expr1014]) WITH UNORDERED PREFETCH)"
--"                 |--Nested Loops(Inner Join, OUTER REFERENCES:([p].[idp]))"
--"                 |    |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1000]))"
--"                 |    |    |--Index Seek(OBJECT:([MIN0150].[dbo].[Product].[idx_product_unit_price] AS [p]), SEEK:([p].[unit_price] >= (20000000) AND [p].[unit_price] <= (20002000)) ORDERED FORWARD)"
--"                 |    |    |--RID Lookup(OBJECT:([MIN0150].[dbo].[Product] AS [p]), SEEK:([Bmk1000]=[Bmk1000]) LOOKUP ORDERED FORWARD)"
--"                 |    |--Index Seek(OBJECT:([MIN0150].[dbo].[OrderItem].[idx_orderitem_idp] AS [oi]), SEEK:([oi].[idp]=[MIN0150].[dbo].[Product].[idp] as [p].[idp]) ORDERED FORWARD)"
--"                 |--RID Lookup(OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD)"
    
-- start 
-- 103 + 495
-- logical reads: 109 + 4
-- cpu time: 661us
SELECT count(*), sum(oi.quantity) FROM Product p
                  INNER JOIN OrderItem oi ON oi.idp = p.idp
WHERE p.unit_price between 20000000 and 20002000
option (maxdop 1);

--vytvoření pokrývajícího indexu, který obsahuje quantity
drop index idx_orderitem_idp on OrderItem;
create index idx_orderitem_idp on dbo.OrderItem(idp) include (quantity);

-- it 1
--"SELECT count(*), sum(oi.quantity) FROM Product p
--        INNER JOIN OrderItem oi ON oi.idp = p.idp
--WHERE p.unit_price between 20000000 and 20002000
--option (maxdop 1)"
--"  |--Compute Scalar(DEFINE:([Expr1004]=CONVERT_IMPLICIT(int,[Expr1013],0), [Expr1005]=CASE WHEN [Expr1013]=(0) THEN NULL ELSE [Expr1014] END))"
--"       |--Stream Aggregate(DEFINE:([Expr1013]=Count(*), [Expr1014]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])))"
--"            |--Nested Loops(Inner Join, OUTER REFERENCES:([p].[idp]))"
--"                 |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1000]))"
--"                 |    |--Index Seek(OBJECT:([MIN0150].[dbo].[Product].[idx_product_unit_price] AS [p]), SEEK:([p].[unit_price] >= (20000000) AND [p].[unit_price] <= (20002000)) ORDERED FORWARD)"
--"                 |    |--RID Lookup(OBJECT:([MIN0150].[dbo].[Product] AS [p]), SEEK:([Bmk1000]=[Bmk1000]) LOOKUP ORDERED FORWARD)"
--"                 |--Index Seek(OBJECT:([MIN0150].[dbo].[OrderItem].[idx_orderitem_idp] AS [oi]), SEEK:([oi].[idp]=[MIN0150].[dbo].[Product].[idp] as [p].[idp]) ORDERED FORWARD)"
-- logical reads: 6 + 4
-- cpu time: 306us

