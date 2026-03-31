-- 25 records
SELECT * FROM PRODUCT p
INNER JOIN ORDERITEM oi ON p.IDP = oi.IDP
INNER JOIN "Order" o ON o.IDO = oi.IDO
WHERE p.NAME LIKE 'Car%' AND
      extract(YEAR from o.ORDER_DATETIME) = 2022 AND
      oi.UNIT_PRICE BETWEEN 1000000 AND 1010000;

-- selektivita
-- (1678 / 100000) * 100 = 1.68 %
-- malá selektivita
SELECT COUNT(*) FROM PRODUCT;
SELECT COUNT(*) FROM PRODUCT WHERE NAME LIKE 'Car%';

-- 19263 / 499894 * 100 = 3.85%
-- malá selektivita
SELECT COUNT(*) FROM "Order";
SELECT COUNT(*) FROM "Order" WHERE extract(YEAR from ORDER_DATETIME) = 2022;

-- 2690 / 5000005 * 100 = 0.054%
-- velká selektivita -> zoptimalizujeme zde fk
SELECT COUNT(*) FROM ORDERITEM;
SELECT COUNT(*) FROM ORDERITEM WHERE UNIT_PRICE BETWEEN 1000000 AND 1010000;

explain plan for
SELECT * FROM PRODUCT p
                  INNER JOIN ORDERITEM oi ON p.IDP = oi.IDP
                  INNER JOIN "Order" o ON o.IDO = oi.IDO
WHERE p.NAME LIKE 'Car%' AND
    extract(YEAR from o.ORDER_DATETIME) = 2022 AND
    oi.UNIT_PRICE BETWEEN 1000000 AND 1010000;

select * from table(dbms_xplan.display);
/*
-----------------------------------------------------------------------------------------------
| Id  | Operation                      | Name         | Rows  | Bytes | Cost (%CPU)| Time     |
-----------------------------------------------------------------------------------------------
|   0 | SELECT STATEMENT               |              |    30 |  2280 |  4479   (3)| 00:00:01 |
|   1 |  NESTED LOOPS                  |              |    30 |  2280 |  4479   (3)| 00:00:01 |
|   2 |   NESTED LOOPS                 |              |    30 |  2280 |  4479   (3)| 00:00:01 |
|   3 |    NESTED LOOPS                |              |    30 |  1440 |  4420   (3)| 00:00:01 |
|*  4 |     TABLE ACCESS FULL          | ORDERITEM    |    30 |   540 |  4390   (3)| 00:00:01 |
|*  5 |     TABLE ACCESS BY INDEX ROWID| PRODUCT      |     1 |    30 |     1   (0)| 00:00:01 |
|*  6 |      INDEX UNIQUE SCAN         | SYS_C0019957 |     1 |       |     0   (0)| 00:00:01 |
|*  7 |    INDEX UNIQUE SCAN           | SYS_C0019974 |     1 |       |     1   (0)| 00:00:01 |
|*  8 |   TABLE ACCESS BY INDEX ROWID  | Order        |     1 |    28 |     2   (0)| 00:00:01 |
-----------------------------------------------------------------------------------------------
 */

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'SELECT * FROM PRODUCT p%';

--io cost: 18737
--cpu time: 250ms
begin
    PrintQueryStat('a0u350pn9gnz1', 262673074);
end;
 
create index idx_order_item_idp on ORDERITEM(IDP);
--create index idx_order_item_ido on ORDERITEM(IDO);

/*
-------------------------------------------------------------------------------------------------------------
| Id  | Operation                              | Name               | Rows  | Bytes | Cost (%CPU)| Time     |
-------------------------------------------------------------------------------------------------------------
|   0 | SELECT STATEMENT                       |                    |    30 |  2280 |  3632   (1)| 00:00:01 |
|   1 |  NESTED LOOPS                          |                    |    30 |  2280 |  3632   (1)| 00:00:01 |
|   2 |   NESTED LOOPS                         |                    |    30 |  2280 |  3632   (1)| 00:00:01 |
|   3 |    NESTED LOOPS                        |                    |    30 |  1440 |  3572   (1)| 00:00:01 |
|*  4 |     TABLE ACCESS FULL                  | PRODUCT            |    66 |  1980 |   138   (2)| 00:00:01 |
|*  5 |     TABLE ACCESS BY INDEX ROWID BATCHED| ORDERITEM          |     1 |    18 |    52   (0)| 00:00:01 |
|*  6 |      INDEX RANGE SCAN                  | IDX_ORDER_ITEM_IDP |    50 |       |     2   (0)| 00:00:01 |
|*  7 |    INDEX UNIQUE SCAN                   | SYS_C0019974       |     1 |       |     1   (0)| 00:00:01 |
|*  8 |   TABLE ACCESS BY INDEX ROWID          | Order              |     1 |    28 |     2   (0)| 00:00:01 |
-------------------------------------------------------------------------------------------------------------

*/
explain plan for
SELECT * FROM PRODUCT p
                  INNER JOIN ORDERITEM oi ON p.IDP = oi.IDP
                  INNER JOIN "Order" o ON o.IDO = oi.IDO
WHERE p.NAME LIKE 'Car%' AND
    extract(YEAR from o.ORDER_DATETIME) = 2022 AND
    oi.UNIT_PRICE BETWEEN 1000000 AND 1010000;

select * from table(dbms_xplan.display);