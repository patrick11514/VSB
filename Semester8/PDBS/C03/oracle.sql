SELECT * FROM ORDERITEM WHERE unit_price < 300;


create or replace procedure PrintQueryStat(p_sql_id varchar2, p_plan_hash_value int)
as
begin
    -- report the statistics of the query processing
    for rec in (
        select * from vsql_user
        where sql_id=p_sql_id and plan_hash_value=p_plan_hash_value
        )
        loop
            dbms_output.put_line('---- Query Processing Statistics ----');
            dbms_output.put_line('executions:               ' || rec.executions);
            dbms_output.put_line('rows_processed_per_exec:  ' || rec.rows_processed_per_exec);
            dbms_output.put_line('buffer_gets_per_exec:     ' || rec.buffer_gets_per_exec);
            dbms_output.put_line('cpu_time_per_exec:        ' || rec.cpu_time_per_exec);
            dbms_output.put_line('cpu_time_per_exec_ms:     ' || round(rec.cpu_time_per_exec/1000, 0));
            dbms_output.put_line('elapsed_time_per_exec:    ' || rec.elapsed_time_per_exec);
            dbms_output.put_line('elapsed_time_per_exec_ms: ' || round(rec.elapsed_time_per_exec/1000, 0));
            dbms_output.put_line('total_elapsed_time_ms:    ' || rec.total_elapsed_time_ms);
            dbms_output.put_line('sql_text: ' || rec.sql_text);
        end loop;
end;

SELECT * FROM vsql_user;

explain plan for SELECT * FROM ORDERITEM WHERE unit_price < 300;

select * from table(dbms_xplan.display);

SELECT * FROM ORDERITEM WHERE unit_price < 300;


SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'SELECT * FROM ORDERITEM WHERE unit_price < 300';

select blocks, bytes/1024/1024 as MB from user_segments
where segment_name = 'ORDERITEM';

begin
    PrintQueryStat('g10326tsnvsvs', 4294024870);
end;

select degree
from user_tables
where table_name='ORDERITEM';

alter table OrderItem parallel (degree 4);

begin
    PrintQueryStat('g10326tsnvsvs', 892791741);
end;

DELETE FROM ORDERITEM WHERE MOD(IDO, 4) = 0;

alter table OrderItem parallel (degree 1);

begin
    PrintQueryStat('g10326tsnvsvs', 4294024870);
end;

alter table OrderItem enable row movement;
alter table OrderItem shrink space;