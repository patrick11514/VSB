/*
 MIN0150
*/

SET SHOWPLAN_ALL ON;
SET SHOWPLAN_ALL OFF;

set statistics io on;
set statistics time on;
set statistics io off;
set statistics time off;

-- 82, 411
SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o
INNER JOIN OrderItem oi ON oi.ido = o.ido
INNER JOIN Staff s ON s.idsa = o.idsa
INNER JOIN Customer c ON c.idc = o.idc
WHERE 
    o.order_datetime >= '2025-01-01' AND 
    o.order_datetime < '2025-02-01' AND
    s.residence = 'Praha' AND
    c.residence = 'Praha'
option (maxdop 1);

-- selektivita
-- 0.46% - velká selektivita (< 1%)
SELECT 
    cast(sum(case when o.order_datetime >= '2025-01-01' AND o.order_datetime < '2025-02-01' then 1 end) as float) / count(*) * 100 
from [dbo].[Order] o;
-- 4.88% - malá selektivita
SELECT 
    cast(sum(case when s.residence = 'Praha' then 1 end) as float) / count(*) * 100 
from Staff s;
-- 5.007% - malá selektivita
SELECT 
    cast(sum(case when c.residence = 'Praha' then 1 end) as float) / count(*) * 100 
from Customer c;


-- 1. iterace
-- QEP
/*
SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o  INNER JOIN OrderItem oi ON oi.ido = o.ido  INNER JOIN Staff s ON s.idsa = o.idsa  INNER JOIN Customer c ON c.idc = o.idc  WHERE       o.order_datetime >= '2025-01-01' AND       o.order_datetime < '2025-02-01' AND      s.residence = 'Praha' AND      c.residence = 'Praha'  option (maxdop 1);	1	1	0	NULL	NULL	1	NULL	1	NULL	NULL	NULL	3.712039	NULL	NULL	SELECT	0	NULL
  |--Compute Scalar(DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1033],0), [Expr1009]=CASE WHEN [Expr1033]=(0) THEN NULL ELSE [Expr1034] END))	1	2	1	Compute Scalar	Compute Scalar	DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1033],0), [Expr1009]=CASE WHEN [Expr1033]=(0) THEN NULL ELSE [Expr1034] END)	[Expr1008]=CONVERT_IMPLICIT(int,[Expr1033],0), [Expr1009]=CASE WHEN [Expr1033]=(0) THEN NULL ELSE [Expr1034] END	1	0	0	15	3.712039	[Expr1008], [Expr1009]	NULL	PLAN_ROW	0	1
       |--Stream Aggregate(DEFINE:([Expr1033]=Count(*), [Expr1034]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])))	1	3	2	Stream Aggregate	Aggregate	NULL	[Expr1033]=Count(*), [Expr1034]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])	1	0	4.179542E-05	15	3.712039	[Expr1033], [Expr1034]	NULL	PLAN_ROW	0	1
            |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1002], [Expr1032]) WITH UNORDERED PREFETCH)	1	4	3	Nested Loops	Inner Join	OUTER REFERENCES:([Bmk1002], [Expr1032]) WITH UNORDERED PREFETCH	NULL	68.8257	0	0.0002876914	11	3.711997	[oi].[quantity]	NULL	PLAN_ROW	0	1
                 |--Nested Loops(Inner Join, OUTER REFERENCES:([o].[ido]))	1	6	4	Nested Loops	Inner Join	OUTER REFERENCES:([o].[ido])	NULL	68.8257	0	0.0002876914	15	3.489267	[Bmk1002]	NULL	PLAN_ROW	0	1
                 |    |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1006], [Expr1031]) WITH UNORDERED PREFETCH)	1	7	6	Nested Loops	Inner Join	OUTER REFERENCES:([Bmk1006], [Expr1031]) WITH UNORDERED PREFETCH	NULL	126.6272	0	0.0005293019	21	3.469399	[o].[ido]	NULL	PLAN_ROW	0	1
                 |    |    |--Nested Loops(Inner Join, OUTER REFERENCES:([o].[idc], [Expr1030]) WITH UNORDERED PREFETCH)	1	9	7	Nested Loops	Inner Join	OUTER REFERENCES:([o].[idc], [Expr1030]) WITH UNORDERED PREFETCH	NULL	126.6272	0	0.0005293019	19	3.071124	[o].[ido], [Bmk1006]	NULL	PLAN_ROW	0	1
                 |    |    |    |--Hash Match(Inner Join, HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])DEFINE:([Opt_Bitmap1012]))	1	11	9	Hash Match	Inner Join	HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])	[Opt_Bitmap1012]	126.6272	0	0.01181245	15	2.692404	[o].[ido], [o].[idc]	NULL	PLAN_ROW	0	1
                 |    |    |    |    |--Table Scan(OBJECT:([MIN0150].[dbo].[Staff] AS [s]),  WHERE:([MIN0150].[dbo].[Staff].[residence] as [s].[residence]='Praha'))	1	12	11	Table Scan	Table Scan	OBJECT:([MIN0150].[dbo].[Staff] AS [s]),  WHERE:([MIN0150].[dbo].[Staff].[residence] as [s].[residence]='Praha')	[s].[idsa]	488	0.05571759	0.011157	24	0.06687459	[s].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |    |    |--Filter(WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])))	1	13	11	Filter	Filter	WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa]))	NULL	222.9303	0	0.4412443	24	2.608914	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |    |         |--Table Scan(OBJECT:([MIN0150].[dbo].[Order] AS [o]),  WHERE:([MIN0150].[dbo].[Order].[order_datetime] as [o].[order_datetime]>='2025-01-01' AND [MIN0150].[dbo].[Order].[order_datetime] as [o].[order_datetime]<'2025-02-01'))	1	14	13	Table Scan	Table Scan	OBJECT:([MIN0150].[dbo].[Order] AS [o]),  WHERE:([MIN0150].[dbo].[Order].[order_datetime] as [o].[order_datetime]>='2025-01-01' AND [MIN0150].[dbo].[Order].[order_datetime] as [o].[order_datetime]<'2025-02-01')	[o].[ido], [o].[idc], [o].[idsa]	222.9303	1.616458	0.551211	27	2.167669	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |    |--Index Seek(OBJECT:([MIN0150].[dbo].[Customer].[PK__Customer__DC501A0C86871272] AS [c]), SEEK:([c].[idc]=[MIN0150].[dbo].[Order].[idc] as [o].[idc]) ORDERED FORWARD)	1	15	9	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Customer].[PK__Customer__DC501A0C86871272] AS [c]), SEEK:([c].[idc]=[MIN0150].[dbo].[Order].[idc] as [o].[idc]) ORDERED FORWARD	[Bmk1006]	1	0.003125	0.0001581	15	0.3781904	[Bmk1006]	NULL	PLAN_ROW	0	126.6272
                 |    |    |--RID Lookup(OBJECT:([MIN0150].[dbo].[Customer] AS [c]), SEEK:([Bmk1006]=[Bmk1006]),  WHERE:([MIN0150].[dbo].[Customer].[residence] as [c].[residence]='Praha') LOOKUP ORDERED FORWARD)	1	17	7	RID Lookup	RID Lookup	OBJECT:([MIN0150].[dbo].[Customer] AS [c]), SEEK:([Bmk1006]=[Bmk1006]),  WHERE:([MIN0150].[dbo].[Customer].[residence] as [c].[residence]='Praha') LOOKUP ORDERED FORWARD	NULL	6.877468	0.003125	0.0001581	21	0.3977466	NULL	NULL	PLAN_ROW	0	126.6272
                 |    |--Index Seek(OBJECT:([MIN0150].[dbo].[OrderItem].[pk_orderitem] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD)	1	18	6	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[OrderItem].[pk_orderitem] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD	[Bmk1002]	10.00742	0.003125	0.0001680082	15	0.01951926	[Bmk1002]	NULL	PLAN_ROW	0	6.877468
                 |--RID Lookup(OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD)	1	20	4	RID Lookup	RID Lookup	OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD	[oi].[quantity]	1	0.003125	0.0001581	11	0.222442	[oi].[quantity]	NULL	PLAN_ROW	0	68.8257
*/
-- Statistiky:
-- CPU Time: 63ms
-- IO Cost: 2179 + 72 + 103 + 468 = 2822
-- Na plánu můžeme vidět, že DBS provádí Table scan na tabulce Order, zároveň v této tabulce provádíme velkou selektivitu 
-- a proto je vhodné zde vytvořit index na atributu order_datetime

-- vytvoření indexu
create index idx_order_order_date_time on [dbo].[Order](order_datetime);

-- 2.iterace
SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o
INNER JOIN OrderItem oi ON oi.ido = o.ido
INNER JOIN Staff s ON s.idsa = o.idsa
INNER JOIN Customer c ON c.idc = o.idc
WHERE 
    o.order_datetime >= '2025-01-01' AND 
    o.order_datetime < '2025-02-01' AND
    s.residence = 'Praha' AND
    c.residence = 'Praha'
option (maxdop 1);

-- QEP
/*
SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o  INNER JOIN OrderItem oi ON oi.ido = o.ido  INNER JOIN Staff s ON s.idsa = o.idsa  INNER JOIN Customer c ON c.idc = o.idc  WHERE       o.order_datetime >= '2025-01-01' AND       o.order_datetime < '2025-02-01' AND      s.residence = 'Praha' AND      c.residence = 'Praha'  option (maxdop 1);	1	1	0	NULL	NULL	1	NULL	1	NULL	NULL	NULL	3.75605	NULL	NULL	SELECT	0	NULL
  |--Compute Scalar(DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1033],0), [Expr1009]=CASE WHEN [Expr1033]=(0) THEN NULL ELSE [Expr1034] END))	1	2	1	Compute Scalar	Compute Scalar	DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1033],0), [Expr1009]=CASE WHEN [Expr1033]=(0) THEN NULL ELSE [Expr1034] END)	[Expr1008]=CONVERT_IMPLICIT(int,[Expr1033],0), [Expr1009]=CASE WHEN [Expr1033]=(0) THEN NULL ELSE [Expr1034] END	1	0	0	15	3.75605	[Expr1008], [Expr1009]	NULL	PLAN_ROW	0	1
       |--Stream Aggregate(DEFINE:([Expr1033]=Count(*), [Expr1034]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])))	1	3	2	Stream Aggregate	Aggregate	NULL	[Expr1033]=Count(*), [Expr1034]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])	1	0	4.364571E-05	15	3.75605	[Expr1033], [Expr1034]	NULL	PLAN_ROW	0	1
            |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1002], [Expr1032]) WITH UNORDERED PREFETCH)	1	4	3	Nested Loops	Inner Join	OUTER REFERENCES:([Bmk1002], [Expr1032]) WITH UNORDERED PREFETCH	NULL	71.90952	0	0.0003005818	11	3.756006	[oi].[quantity]	NULL	PLAN_ROW	0	1
                 |--Nested Loops(Inner Join, OUTER REFERENCES:([o].[ido]))	1	6	4	Nested Loops	Inner Join	OUTER REFERENCES:([o].[ido])	NULL	71.90952	0	0.0003005818	15	3.523176	[Bmk1002]	NULL	PLAN_ROW	0	1
                 |    |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1006], [Expr1031]) WITH UNORDERED PREFETCH)	1	7	6	Nested Loops	Inner Join	OUTER REFERENCES:([Bmk1006], [Expr1031]) WITH UNORDERED PREFETCH	NULL	132.3009	0	0.0005530178	21	3.502279	[o].[ido]	NULL	PLAN_ROW	0	1
                 |    |    |--Nested Loops(Inner Join, OUTER REFERENCES:([o].[idc], [Expr1030]) WITH UNORDERED PREFETCH)	1	9	7	Nested Loops	Inner Join	OUTER REFERENCES:([o].[idc], [Expr1030]) WITH UNORDERED PREFETCH	NULL	132.3009	0	0.0005530178	19	3.086711	[o].[ido], [Bmk1006]	NULL	PLAN_ROW	0	1
                 |    |    |    |--Hash Match(Inner Join, HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])DEFINE:([Opt_Bitmap1012]))	1	11	9	Hash Match	Inner Join	HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])	[Opt_Bitmap1012]	132.3009	0	0.01183747	15	2.692429	[o].[ido], [o].[idc]	NULL	PLAN_ROW	0	1
                 |    |    |    |    |--Table Scan(OBJECT:([MIN0150].[dbo].[Staff] AS [s]),  WHERE:([MIN0150].[dbo].[Staff].[residence] as [s].[residence]='Praha'))	1	12	11	Table Scan	Table Scan	OBJECT:([MIN0150].[dbo].[Staff] AS [s]),  WHERE:([MIN0150].[dbo].[Staff].[residence] as [s].[residence]='Praha')	[s].[idsa]	488	0.05571759	0.011157	24	0.06687459	[s].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |    |    |--Filter(WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])))	1	13	11	Filter	Filter	WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa]))	NULL	232.9189	0	0.4412443	24	2.608914	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |    |         |--Table Scan(OBJECT:([MIN0150].[dbo].[Order] AS [o]),  WHERE:([MIN0150].[dbo].[Order].[order_datetime] as [o].[order_datetime]>='2025-01-01' AND [MIN0150].[dbo].[Order].[order_datetime] as [o].[order_datetime]<'2025-02-01'))	1	14	13	Table Scan	Table Scan	OBJECT:([MIN0150].[dbo].[Order] AS [o]),  WHERE:([MIN0150].[dbo].[Order].[order_datetime] as [o].[order_datetime]>='2025-01-01' AND [MIN0150].[dbo].[Order].[order_datetime] as [o].[order_datetime]<'2025-02-01')	[o].[ido], [o].[idc], [o].[idsa]	232.9189	1.616458	0.551211	27	2.167669	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |    |--Index Seek(OBJECT:([MIN0150].[dbo].[Customer].[PK__Customer__DC501A0C86871272] AS [c]), SEEK:([c].[idc]=[MIN0150].[dbo].[Order].[idc] as [o].[idc]) ORDERED FORWARD)	1	15	9	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Customer].[PK__Customer__DC501A0C86871272] AS [c]), SEEK:([c].[idc]=[MIN0150].[dbo].[Order].[idc] as [o].[idc]) ORDERED FORWARD	[Bmk1006]	1	0.003125	0.0001581	15	0.3937288	[Bmk1006]	NULL	PLAN_ROW	0	132.3009
                 |    |    |--RID Lookup(OBJECT:([MIN0150].[dbo].[Customer] AS [c]), SEEK:([Bmk1006]=[Bmk1006]),  WHERE:([MIN0150].[dbo].[Customer].[residence] as [c].[residence]='Praha') LOOKUP ORDERED FORWARD)	1	17	7	RID Lookup	RID Lookup	OBJECT:([MIN0150].[dbo].[Customer] AS [c]), SEEK:([Bmk1006]=[Bmk1006]),  WHERE:([MIN0150].[dbo].[Customer].[residence] as [c].[residence]='Praha') LOOKUP ORDERED FORWARD	NULL	7.185621	0.003125	0.0001581	21	0.4150147	NULL	NULL	PLAN_ROW	0	132.3009
                 |    |--Index Seek(OBJECT:([MIN0150].[dbo].[OrderItem].[pk_orderitem] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD)	1	18	6	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[OrderItem].[pk_orderitem] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD	[Bmk1002]	10.00742	0.003125	0.0001680082	15	0.02053362	[Bmk1002]	NULL	PLAN_ROW	0	7.185621
                 |--RID Lookup(OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD)	1	20	4	RID Lookup	RID Lookup	OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD	[oi].[quantity]	1	0.003125	0.0001581	11	0.2325295	[oi].[quantity]	NULL	PLAN_ROW	0	71.90952
*/
-- Statistiky:
-- CPU Time: 47ms
-- IO Cost: 2179 + 72 + 103 + 468 = 2822
-- Z plánu můžeme vidět, že DBS index nevyužil, tím že na tabulce order děláme mnoho spojení, je vhodné do indexu také zařadit atributy, které
-- se ve spojeních používají, index tedy dáme pryč a vytvoříme pokrývající index

drop index idx_order_order_date_time on [dbo].[Order];
-- vzhledem k počtu atritubů v pokrývaném indexu by bylo za zvážení vytvoření clustered table
create index idx_order_order_date_time on [dbo].[Order](order_datetime) include (ido, idsa, idc);

-- 3.iterace
SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o
INNER JOIN OrderItem oi ON oi.ido = o.ido
INNER JOIN Staff s ON s.idsa = o.idsa
INNER JOIN Customer c ON c.idc = o.idc
WHERE 
    o.order_datetime >= '2025-01-01' AND 
    o.order_datetime < '2025-02-01' AND
    s.residence = 'Praha' AND
    c.residence = 'Praha'
option (maxdop 1);

--QEP
/*
SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o  INNER JOIN OrderItem oi ON oi.ido = o.ido  INNER JOIN Staff s ON s.idsa = o.idsa  INNER JOIN Customer c ON c.idc = o.idc  WHERE       o.order_datetime >= '2025-01-01' AND       o.order_datetime < '2025-02-01' AND      s.residence = 'Praha' AND      c.residence = 'Praha'  option (maxdop 1);	1	1	0	NULL	NULL	1	NULL	1	NULL	NULL	NULL	1.159645	NULL	NULL	SELECT	0	NULL
  |--Compute Scalar(DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1023],0), [Expr1009]=CASE WHEN [Expr1023]=(0) THEN NULL ELSE [Expr1024] END))	1	2	1	Compute Scalar	Compute Scalar	DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1023],0), [Expr1009]=CASE WHEN [Expr1023]=(0) THEN NULL ELSE [Expr1024] END)	[Expr1008]=CONVERT_IMPLICIT(int,[Expr1023],0), [Expr1009]=CASE WHEN [Expr1023]=(0) THEN NULL ELSE [Expr1024] END	1	0	0	15	1.159645	[Expr1008], [Expr1009]	NULL	PLAN_ROW	0	1
       |--Stream Aggregate(DEFINE:([Expr1023]=Count(*), [Expr1024]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])))	1	3	2	Stream Aggregate	Aggregate	NULL	[Expr1023]=Count(*), [Expr1024]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])	1	0	4.364571E-05	15	1.159645	[Expr1023], [Expr1024]	NULL	PLAN_ROW	0	1
            |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1002], [Expr1022]) WITH UNORDERED PREFETCH)	1	4	3	Nested Loops	Inner Join	OUTER REFERENCES:([Bmk1002], [Expr1022]) WITH UNORDERED PREFETCH	NULL	71.90952	0	0.0003005818	11	1.159601	[oi].[quantity]	NULL	PLAN_ROW	0	1
                 |--Nested Loops(Inner Join, OUTER REFERENCES:([o].[ido]))	1	6	4	Nested Loops	Inner Join	OUTER REFERENCES:([o].[ido])	NULL	71.90952	0	0.0003005818	15	0.926771	[Bmk1002]	NULL	PLAN_ROW	0	1
                 |    |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1006], [Expr1021]) WITH UNORDERED PREFETCH)	1	7	6	Nested Loops	Inner Join	OUTER REFERENCES:([Bmk1006], [Expr1021]) WITH UNORDERED PREFETCH	NULL	132.3009	0	0.0005530178	21	0.9058734	[o].[ido]	NULL	PLAN_ROW	0	1
                 |    |    |--Nested Loops(Inner Join, OUTER REFERENCES:([o].[idc], [Expr1020]) WITH UNORDERED PREFETCH)	1	9	7	Nested Loops	Inner Join	OUTER REFERENCES:([o].[idc], [Expr1020]) WITH UNORDERED PREFETCH	NULL	132.3009	0	0.0005530178	19	0.4903056	[o].[ido], [Bmk1006]	NULL	PLAN_ROW	0	1
                 |    |    |    |--Hash Match(Inner Join, HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])DEFINE:([Opt_Bitmap1012]))	1	11	9	Hash Match	Inner Join	HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])	[Opt_Bitmap1012]	132.3009	0	0.01183747	15	0.09602351	[o].[ido], [o].[idc]	NULL	PLAN_ROW	0	1
                 |    |    |    |    |--Table Scan(OBJECT:([MIN0150].[dbo].[Staff] AS [s]),  WHERE:([MIN0150].[dbo].[Staff].[residence] as [s].[residence]='Praha'))	1	12	11	Table Scan	Table Scan	OBJECT:([MIN0150].[dbo].[Staff] AS [s]),  WHERE:([MIN0150].[dbo].[Staff].[residence] as [s].[residence]='Praha')	[s].[idsa]	488	0.05571759	0.011157	24	0.06687459	[s].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |    |    |--Filter(WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])))	1	13	11	Filter	Filter	WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa]))	NULL	232.9189	0.009791667	0.002716779	24	0.01250845	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |    |         |--Index Seek(OBJECT:([MIN0150].[dbo].[Order].[idx_order_order_date_time] AS [o]), SEEK:([o].[order_datetime] >= '2025-01-01' AND [o].[order_datetime] < '2025-02-01') ORDERED FORWARD)	1	14	13	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Order].[idx_order_order_date_time] AS [o]), SEEK:([o].[order_datetime] >= '2025-01-01' AND [o].[order_datetime] < '2025-02-01') ORDERED FORWARD	[o].[ido], [o].[idc], [o].[idsa]	232.9189	0.009791667	0.002716779	24	0.01250845	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |    |--Index Seek(OBJECT:([MIN0150].[dbo].[Customer].[PK__Customer__DC501A0C86871272] AS [c]), SEEK:([c].[idc]=[MIN0150].[dbo].[Order].[idc] as [o].[idc]) ORDERED FORWARD)	1	15	9	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Customer].[PK__Customer__DC501A0C86871272] AS [c]), SEEK:([c].[idc]=[MIN0150].[dbo].[Order].[idc] as [o].[idc]) ORDERED FORWARD	[Bmk1006]	1	0.003125	0.0001581	15	0.3937288	[Bmk1006]	NULL	PLAN_ROW	0	132.3009
                 |    |    |--RID Lookup(OBJECT:([MIN0150].[dbo].[Customer] AS [c]), SEEK:([Bmk1006]=[Bmk1006]),  WHERE:([MIN0150].[dbo].[Customer].[residence] as [c].[residence]='Praha') LOOKUP ORDERED FORWARD)	1	17	7	RID Lookup	RID Lookup	OBJECT:([MIN0150].[dbo].[Customer] AS [c]), SEEK:([Bmk1006]=[Bmk1006]),  WHERE:([MIN0150].[dbo].[Customer].[residence] as [c].[residence]='Praha') LOOKUP ORDERED FORWARD	NULL	7.185621	0.003125	0.0001581	21	0.4150147	NULL	NULL	PLAN_ROW	0	132.3009
                 |    |--Index Seek(OBJECT:([MIN0150].[dbo].[OrderItem].[pk_orderitem] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD)	1	18	6	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[OrderItem].[pk_orderitem] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD	[Bmk1002]	10.00742	0.003125	0.0001680082	15	0.02053362	[Bmk1002]	NULL	PLAN_ROW	0	7.185621
                 |--RID Lookup(OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD)	1	20	4	RID Lookup	RID Lookup	OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD	[oi].[quantity]	1	0.003125	0.0001581	11	0.2325295	[oi].[quantity]	NULL	PLAN_ROW	0	71.90952
*/
-- Statistiky:
--cpu time = 0 ->
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
where query_text like 'SELECT SUM(oi.quantity), COUNT(*) FROM %'
order by avg_cpu_time desc;

-- CPU Time: 9,9ms
-- IO Cost: 14 + 72 + 103 + 468 = 657
-- Nyní DBS již index využil a IO cost pro danou tabulku se snížil, nyní DBS provádí RID lookupy do tabulky customer, vytvoříme proto index 
-- pro atribut residence a opět pokrývající, ať již b-strom obsahuje idc vedle atributu residence

-- vytvoření indexu
create index idx_customer_residence on Customer(residence) include (idc);

-- 4. iterace

SET SHOWPLAN_ALL ON;
SET SHOWPLAN_ALL OFF;

set statistics io on;
set statistics time on;
set statistics io off;
set statistics time off;


SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o
INNER JOIN OrderItem oi ON oi.ido = o.ido
INNER JOIN Staff s ON s.idsa = o.idsa
INNER JOIN Customer c ON c.idc = o.idc
WHERE 
    o.order_datetime >= '2025-01-01' AND 
    o.order_datetime < '2025-02-01' AND
    s.residence = 'Praha' AND
    c.residence = 'Praha'
option (maxdop 1);

--QEP
/*
SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o  INNER JOIN OrderItem oi ON oi.ido = o.ido  INNER JOIN Staff s ON s.idsa = o.idsa  INNER JOIN Customer c ON c.idc = o.idc  WHERE       o.order_datetime >= '2025-01-01' AND       o.order_datetime < '2025-02-01' AND      s.residence = 'Praha' AND      c.residence = 'Praha'  option (maxdop 1);	1	1	0	NULL	NULL	1	NULL	1	NULL	NULL	NULL	0.4169467	NULL	NULL	SELECT	0	NULL
  |--Compute Scalar(DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1020],0), [Expr1009]=CASE WHEN [Expr1020]=(0) THEN NULL ELSE [Expr1021] END))	1	2	1	Compute Scalar	Compute Scalar	DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1020],0), [Expr1009]=CASE WHEN [Expr1020]=(0) THEN NULL ELSE [Expr1021] END)	[Expr1008]=CONVERT_IMPLICIT(int,[Expr1020],0), [Expr1009]=CASE WHEN [Expr1020]=(0) THEN NULL ELSE [Expr1021] END	1	0	0	15	0.4169467	[Expr1008], [Expr1009]	NULL	PLAN_ROW	0	1
       |--Stream Aggregate(DEFINE:([Expr1020]=Count(*), [Expr1021]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])))	1	3	2	Stream Aggregate	Aggregate	NULL	[Expr1020]=Count(*), [Expr1021]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])	1	0	4.345176E-05	15	0.4169467	[Expr1020], [Expr1021]	NULL	PLAN_ROW	0	1
            |--Nested Loops(Inner Join, OUTER REFERENCES:([Bmk1002], [Expr1019]) WITH UNORDERED PREFETCH)	1	4	3	Nested Loops	Inner Join	OUTER REFERENCES:([Bmk1002], [Expr1019]) WITH UNORDERED PREFETCH	NULL	71.58626	0	0.0002992306	11	0.4169032	[oi].[quantity]	NULL	PLAN_ROW	0	1
                 |--Nested Loops(Inner Join, OUTER REFERENCES:([o].[ido]))	1	6	4	Nested Loops	Inner Join	OUTER REFERENCES:([o].[ido])	NULL	71.58626	0	0.0002992306	15	0.1851318	[Bmk1002]	NULL	PLAN_ROW	0	1
                 |    |--Hash Match(Inner Join, HASH:([o].[idc])=([c].[idc])DEFINE:([Opt_Bitmap1013]))	1	7	6	Hash Match	Inner Join	HASH:([o].[idc])=([c].[idc])	[Opt_Bitmap1013]	7.153319	0	0.00968143	11	0.1644053	[o].[ido]	NULL	PLAN_ROW	0	1
                 |    |    |--Hash Match(Inner Join, HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])DEFINE:([Opt_Bitmap1012]))	1	8	7	Hash Match	Inner Join	HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])	[Opt_Bitmap1012]	132.3009	0	0.01183747	15	0.09602351	[o].[ido], [o].[idc]	NULL	PLAN_ROW	0	1
                 |    |    |    |--Table Scan(OBJECT:([MIN0150].[dbo].[Staff] AS [s]),  WHERE:([MIN0150].[dbo].[Staff].[residence] as [s].[residence]='Praha'))	1	9	8	Table Scan	Table Scan	OBJECT:([MIN0150].[dbo].[Staff] AS [s]),  WHERE:([MIN0150].[dbo].[Staff].[residence] as [s].[residence]='Praha')	[s].[idsa]	488	0.05571759	0.011157	24	0.06687459	[s].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |    |--Filter(WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])))	1	10	8	Filter	Filter	WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa]))	NULL	232.9189	0.009791667	0.002716779	24	0.01250845	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |         |--Index Seek(OBJECT:([MIN0150].[dbo].[Order].[idx_order_order_date_time] AS [o]), SEEK:([o].[order_datetime] >= '2025-01-01' AND [o].[order_datetime] < '2025-02-01') ORDERED FORWARD)	1	11	10	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Order].[idx_order_order_date_time] AS [o]), SEEK:([o].[order_datetime] >= '2025-01-01' AND [o].[order_datetime] < '2025-02-01') ORDERED FORWARD	[o].[ido], [o].[idc], [o].[idsa]	232.9189	0.009791667	0.002716779	24	0.01250845	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |--Index Seek(OBJECT:([MIN0150].[dbo].[Customer].[idx_customer_residence] AS [c]), SEEK:([c].[residence]='Praha')  WHERE:(PROBE([Opt_Bitmap1013],[MIN0150].[dbo].[Customer].[idc] as [c].[idc])) ORDERED FORWARD)	1	13	7	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Customer].[idx_customer_residence] AS [c]), SEEK:([c].[residence]='Praha')  WHERE:(PROBE([Opt_Bitmap1013],[MIN0150].[dbo].[Customer].[idc] as [c].[idc])) ORDERED FORWARD	[c].[idc]	15.021	0.04203228	0.01666508	11	0.05869736	[c].[idc]	NULL	PLAN_ROW	0	1
                 |    |--Index Seek(OBJECT:([MIN0150].[dbo].[OrderItem].[pk_orderitem] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD)	1	14	6	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[OrderItem].[pk_orderitem] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD	[Bmk1002]	10.00742	0.003125	0.0001680082	15	0.02042729	[Bmk1002]	NULL	PLAN_ROW	0	7.153319
                 |--RID Lookup(OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD)	1	16	4	RID Lookup	RID Lookup	OBJECT:([MIN0150].[dbo].[OrderItem] AS [oi]), SEEK:([Bmk1002]=[Bmk1002]) LOOKUP ORDERED FORWARD	[oi].[quantity]	1	0.003125	0.0001581	11	0.2314722	[oi].[quantity]	NULL	PLAN_ROW	0	71.58626
*/
-- Statistiky:
-- CPU Time: 15ms - CPU time se zvýšil, ale to může být způsobeno zátéží během testu
-- IO Cost: 54 + 14 + 72 + 103 = 243
-- Z plánu můžeme vidět, že jsme eliminovali daný RID lookup, nyní v QEP je ještě jeden z RID lookupů do tabulky
-- OrderItem, kvůli atributu quantity, vytvoříme tedy pokrývající index na cizím klíči, který obsahuje atribut quantity

create index idx_orderitem_quantity on OrderItem(ido) include (quantity);


--5.iterace
SET SHOWPLAN_ALL ON;
SET SHOWPLAN_ALL OFF;

set statistics io on;
set statistics time on;
set statistics io off;
set statistics time off;


SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o
INNER JOIN OrderItem oi ON oi.ido = o.ido
INNER JOIN Staff s ON s.idsa = o.idsa
INNER JOIN Customer c ON c.idc = o.idc
WHERE 
    o.order_datetime >= '2025-01-01' AND 
    o.order_datetime < '2025-02-01' AND
    s.residence = 'Praha' AND
    c.residence = 'Praha'
option (maxdop 1);

--QEP
/*
SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o  INNER JOIN OrderItem oi ON oi.ido = o.ido  INNER JOIN Staff s ON s.idsa = o.idsa  INNER JOIN Customer c ON c.idc = o.idc  WHERE       o.order_datetime >= '2025-01-01' AND       o.order_datetime < '2025-02-01' AND      s.residence = 'Praha' AND      c.residence = 'Praha'  option (maxdop 1);	1	1	0	NULL	NULL	1	NULL	1	NULL	NULL	NULL	0.1851856	NULL	NULL	SELECT	0	NULL
  |--Compute Scalar(DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1019],0), [Expr1009]=CASE WHEN [Expr1019]=(0) THEN NULL ELSE [Expr1020] END))	1	2	1	Compute Scalar	Compute Scalar	DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1019],0), [Expr1009]=CASE WHEN [Expr1019]=(0) THEN NULL ELSE [Expr1020] END)	[Expr1008]=CONVERT_IMPLICIT(int,[Expr1019],0), [Expr1009]=CASE WHEN [Expr1019]=(0) THEN NULL ELSE [Expr1020] END	1	0	0	15	0.1851856	[Expr1008], [Expr1009]	NULL	PLAN_ROW	0	1
       |--Stream Aggregate(DEFINE:([Expr1019]=Count(*), [Expr1020]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])))	1	3	2	Stream Aggregate	Aggregate	NULL	[Expr1019]=Count(*), [Expr1020]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])	1	0	4.450695E-05	15	0.1851856	[Expr1019], [Expr1020]	NULL	PLAN_ROW	0	1
            |--Nested Loops(Inner Join, OUTER REFERENCES:([o].[ido]))	1	4	3	Nested Loops	Inner Join	OUTER REFERENCES:([o].[ido])	NULL	73.34492	0	0.0003065818	11	0.1851411	[oi].[quantity]	NULL	PLAN_ROW	0	1
                 |--Hash Match(Inner Join, HASH:([o].[idc])=([c].[idc])DEFINE:([Opt_Bitmap1013]))	1	5	4	Hash Match	Inner Join	HASH:([o].[idc])=([c].[idc])	[Opt_Bitmap1013]	7.153319	0	0.00968143	11	0.1644053	[o].[ido]	NULL	PLAN_ROW	0	1
                 |    |--Hash Match(Inner Join, HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])DEFINE:([Opt_Bitmap1012]))	1	6	5	Hash Match	Inner Join	HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])	[Opt_Bitmap1012]	132.3009	0	0.01183747	15	0.09602351	[o].[ido], [o].[idc]	NULL	PLAN_ROW	0	1
                 |    |    |--Table Scan(OBJECT:([MIN0150].[dbo].[Staff] AS [s]),  WHERE:([MIN0150].[dbo].[Staff].[residence] as [s].[residence]='Praha'))	1	7	6	Table Scan	Table Scan	OBJECT:([MIN0150].[dbo].[Staff] AS [s]),  WHERE:([MIN0150].[dbo].[Staff].[residence] as [s].[residence]='Praha')	[s].[idsa]	488	0.05571759	0.011157	24	0.06687459	[s].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |--Filter(WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])))	1	8	6	Filter	Filter	WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa]))	NULL	232.9189	0.009791667	0.002716779	24	0.01250845	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |         |--Index Seek(OBJECT:([MIN0150].[dbo].[Order].[idx_order_order_date_time] AS [o]), SEEK:([o].[order_datetime] >= '2025-01-01' AND [o].[order_datetime] < '2025-02-01') ORDERED FORWARD)	1	9	8	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Order].[idx_order_order_date_time] AS [o]), SEEK:([o].[order_datetime] >= '2025-01-01' AND [o].[order_datetime] < '2025-02-01') ORDERED FORWARD	[o].[ido], [o].[idc], [o].[idsa]	232.9189	0.009791667	0.002716779	24	0.01250845	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |--Index Seek(OBJECT:([MIN0150].[dbo].[Customer].[idx_customer_residence] AS [c]), SEEK:([c].[residence]='Praha')  WHERE:(PROBE([Opt_Bitmap1013],[MIN0150].[dbo].[Customer].[idc] as [c].[idc])) ORDERED FORWARD)	1	11	5	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Customer].[idx_customer_residence] AS [c]), SEEK:([c].[residence]='Praha')  WHERE:(PROBE([Opt_Bitmap1013],[MIN0150].[dbo].[Customer].[idc] as [c].[idc])) ORDERED FORWARD	[c].[idc]	15.021	0.04203228	0.01666508	11	0.05869736	[c].[idc]	NULL	PLAN_ROW	0	1
                 |--Index Seek(OBJECT:([MIN0150].[dbo].[OrderItem].[idx_orderitem_quantity] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD)	1	12	4	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[OrderItem].[idx_orderitem_quantity] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD	[oi].[quantity]	10.25327	0.003125	0.0001682786	11	0.02042923	[oi].[quantity]	NULL	PLAN_ROW	0	7.153319
*/
-- Statistiky:
-- CPU Time: 15ms
-- IO Cost: 54 + 14 + 72 + 21 = 161
-- Z plánu můžeme vidět, že jsme se zbavili dalšího RID lookupu, poslední optimalizací se můžeme zbavit Table Scanu (kterého jsem si nevšiml :))
-- na tabulce Staff, zde můžeme provést stejnou optimalizaci jako na tabulce Customer s pokrývajícím indexem, kde vytvoříme index
-- na atributu residence a bude zároveň pokrývat i primární klíč

create index idx_staff_residence on Staff(residence) include (idsa);

--6.iterace
SET SHOWPLAN_ALL ON;
SET SHOWPLAN_ALL OFF;

set statistics io on;
set statistics time on;
set statistics io off;
set statistics time off;


SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o
INNER JOIN OrderItem oi ON oi.ido = o.ido
INNER JOIN Staff s ON s.idsa = o.idsa
INNER JOIN Customer c ON c.idc = o.idc
WHERE 
    o.order_datetime >= '2025-01-01' AND 
    o.order_datetime < '2025-02-01' AND
    s.residence = 'Praha' AND
    c.residence = 'Praha'
option (maxdop 1);

--QEP
/*
SELECT COUNT(*), SUM(oi.quantity) FROM [dbo].[Order] o  INNER JOIN OrderItem oi ON oi.ido = o.ido  INNER JOIN Staff s ON s.idsa = o.idsa  INNER JOIN Customer c ON c.idc = o.idc  WHERE       o.order_datetime >= '2025-01-01' AND       o.order_datetime < '2025-02-01' AND      s.residence = 'Praha' AND      c.residence = 'Praha'  option (maxdop 1);	1	1	0	NULL	NULL	1	NULL	1	NULL	NULL	NULL	0.118035	NULL	NULL	SELECT	0	NULL
  |--Compute Scalar(DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1019],0), [Expr1009]=CASE WHEN [Expr1019]=(0) THEN NULL ELSE [Expr1020] END))	1	2	1	Compute Scalar	Compute Scalar	DEFINE:([Expr1008]=CONVERT_IMPLICIT(int,[Expr1019],0), [Expr1009]=CASE WHEN [Expr1019]=(0) THEN NULL ELSE [Expr1020] END)	[Expr1008]=CONVERT_IMPLICIT(int,[Expr1019],0), [Expr1009]=CASE WHEN [Expr1019]=(0) THEN NULL ELSE [Expr1020] END	1	0	0	15	0.118035	[Expr1008], [Expr1009]	NULL	PLAN_ROW	0	1
       |--Stream Aggregate(DEFINE:([Expr1019]=Count(*), [Expr1020]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])))	1	3	2	Stream Aggregate	Aggregate	NULL	[Expr1019]=Count(*), [Expr1020]=SUM([MIN0150].[dbo].[OrderItem].[quantity] as [oi].[quantity])	1	0	4.450695E-05	15	0.118035	[Expr1019], [Expr1020]	NULL	PLAN_ROW	0	1
            |--Nested Loops(Inner Join, OUTER REFERENCES:([o].[ido]))	1	4	3	Nested Loops	Inner Join	OUTER REFERENCES:([o].[ido])	NULL	73.34492	0	0.0003065818	11	0.1179905	[oi].[quantity]	NULL	PLAN_ROW	0	1
                 |--Hash Match(Inner Join, HASH:([o].[idc])=([c].[idc])DEFINE:([Opt_Bitmap1013]))	1	5	4	Hash Match	Inner Join	HASH:([o].[idc])=([c].[idc])	[Opt_Bitmap1013]	7.153319	0	0.00968143	11	0.09725469	[o].[ido]	NULL	PLAN_ROW	0	1
                 |    |--Hash Match(Inner Join, HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])DEFINE:([Opt_Bitmap1012]))	1	6	5	Hash Match	Inner Join	HASH:([s].[idsa])=([o].[idsa]), RESIDUAL:([MIN0150].[dbo].[Staff].[idsa] as [s].[idsa]=[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])	[Opt_Bitmap1012]	132.3009	0	0.01183747	15	0.0288729	[o].[ido], [o].[idc]	NULL	PLAN_ROW	0	1
                 |    |    |--Index Seek(OBJECT:([MIN0150].[dbo].[Staff].[idx_staff_residence] AS [s]), SEEK:([s].[residence]='Praha') ORDERED FORWARD)	1	7	6	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Staff].[idx_staff_residence] AS [s]), SEEK:([s].[residence]='Praha') ORDERED FORWARD	[s].[idsa]	488	0.003830185	0.0006938	16	0.004523985	[s].[idsa]	NULL	PLAN_ROW	0	1
                 |    |    |--Filter(WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa])))	1	8	6	Filter	Filter	WHERE:(PROBE([Opt_Bitmap1012],[MIN0150].[dbo].[Order].[idsa] as [o].[idsa]))	NULL	232.9189	0.009791667	0.002716779	24	0.01250845	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |         |--Index Seek(OBJECT:([MIN0150].[dbo].[Order].[idx_order_order_date_time] AS [o]), SEEK:([o].[order_datetime] >= '2025-01-01' AND [o].[order_datetime] < '2025-02-01') ORDERED FORWARD)	1	9	8	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Order].[idx_order_order_date_time] AS [o]), SEEK:([o].[order_datetime] >= '2025-01-01' AND [o].[order_datetime] < '2025-02-01') ORDERED FORWARD	[o].[ido], [o].[idc], [o].[idsa]	232.9189	0.009791667	0.002716779	24	0.01250845	[o].[ido], [o].[idc], [o].[idsa]	NULL	PLAN_ROW	0	1
                 |    |--Index Seek(OBJECT:([MIN0150].[dbo].[Customer].[idx_customer_residence] AS [c]), SEEK:([c].[residence]='Praha')  WHERE:(PROBE([Opt_Bitmap1013],[MIN0150].[dbo].[Customer].[idc] as [c].[idc])) ORDERED FORWARD)	1	11	5	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[Customer].[idx_customer_residence] AS [c]), SEEK:([c].[residence]='Praha')  WHERE:(PROBE([Opt_Bitmap1013],[MIN0150].[dbo].[Customer].[idc] as [c].[idc])) ORDERED FORWARD	[c].[idc]	15.021	0.04203228	0.01666508	11	0.05869736	[c].[idc]	NULL	PLAN_ROW	0	1
                 |--Index Seek(OBJECT:([MIN0150].[dbo].[OrderItem].[idx_orderitem_quantity] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD)	1	12	4	Index Seek	Index Seek	OBJECT:([MIN0150].[dbo].[OrderItem].[idx_orderitem_quantity] AS [oi]), SEEK:([oi].[ido]=[MIN0150].[dbo].[Order].[ido] as [o].[ido]) ORDERED FORWARD	[oi].[quantity]	10.25327	0.003125	0.0001682786	11	0.02042923	[oi].[quantity]	NULL	PLAN_ROW	0	7.153319
*/
-- Statistiky:
-- CPU Time: 16ms
-- IO Cost: 54 + 14 + 4 + 21 = 93
-- Z plánu zmizel zmíněný Table Scan, jak můžeme vidět CPU time se drží kolem ~15 ms a v průběhu optimalizace se zvedl z minima ~9ms což je ale dle mě
-- nerelevantní změna a nejvíce zde právě provedla první optimalizace přidáním indexu na atribut Order.order_datetime
-- dále si myslím, že kromě navrhované shlukované tabulky na tabulce order jiné optimalizace nenajdu.

drop index idx_order_order_date_time on [dbo].[Order];
drop index idx_customer_residence on Customer;
drop index idx_orderitem_quantity on OrderItem;
drop index idx_staff_residence on Staff;