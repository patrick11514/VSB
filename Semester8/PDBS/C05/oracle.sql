create or replace procedure PrintPages(
    p_table_name varchar,  p_user_name varchar, p_type varchar)
as
    blocks             number;
    bytes              number;
    unused_blocks      number;
    unused_bytes       number;
    expired_blocks     number;
    expired_bytes      number;
    unexpired_blocks   number;
    unexpired_bytes    number;
    unformatted_blocks number;
    unformatted_bytes  number;
    fs1_blocks         number;
    fs1_bytes          number;
    fs2_blocks         number;
    fs2_bytes          number;
    fs3_blocks         number;
    fs3_bytes          number;
    fs4_blocks         number;
    fs4_bytes          number;
    full_blocks        number;
    full_bytes         number;
    used_blocks        number;
    used_bytes         number;
    mega number := 1024.0 * 1024.0;
begin
    dbms_space.unused_space(p_user_name, p_table_name, p_type,
                            blocks, bytes, unused_blocks, unused_bytes, expired_blocks,
                            expired_bytes, unexpired_blocks, unexpired_bytes);

    dbms_space.space_usage(p_user_name, p_table_name, p_type,
                           unformatted_blocks, unformatted_bytes, fs1_blocks, fs1_bytes,
                           fs2_blocks, fs2_bytes, fs3_blocks, fs3_bytes, fs4_blocks,
                           fs4_bytes, full_blocks, full_bytes, null);

    used_blocks := fs1_blocks + fs2_blocks + fs3_blocks + fs4_blocks + full_blocks;
    used_bytes := fs1_bytes + fs2_bytes + fs3_bytes + fs4_bytes + full_bytes;

    dbms_output.put_line('Allocated blocks: ' || blocks || ' (' || trunc(bytes / mega, 1) || 'MB)');
    dbms_output.put_line('Used blocks: ' || used_blocks || ' (' || trunc(used_bytes / mega, 1) || 'MB)');
end;

begin
    PrintPages('CUSTOMER', 'MIN0150', 'TABLE');
end;

select index_name, blevel, leaf_blocks
from user_indexes where table_name='CUSTOMER';

begin
    PrintPages('SYS_C0019953', 'MIN0150', 'INDEX');
end;


-- 2

create table Customer_ct (
                          idc int primary key,
                          fname varchar(20) not null,
                          lname varchar(30) not null,
                          residence varchar(20) not null,
                          gender char(1) not null,
                          birthday date not null
) organization index;

INSERT INTO CUSTOMER_CT SELECT * FROM CUSTOMER;

--3
begin
    PrintPages('CUSTOMER_CT', 'MIN0150', 'TABLE');
end;

select index_name, blevel, leaf_blocks
from user_indexes where table_name='CUSTOMER_CT';

begin
    PrintPages('SYS_IOT_TOP_114650', 'MIN0150', 'INDEX');
end;

--4
alter table CUSTOMER_CT shrink space;

begin
    PrintPages('CUSTOMER_CT', 'MIN0150', 'TABLE');
end;

begin
    PrintPages('SYS_IOT_TOP_114650', 'MIN0150', 'INDEX');
end;

--5
SELECT * FROM CUSTOMER_CT WHERE idc = 10;

explain plan for
SELECT * FROM CUSTOMER_CT WHERE idc = 10;

select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'SELECT * FROM CUSTOMER_CT WHERE idc = 10%';

begin
    PrintQueryStat('46mymbb2unrmw', 3329632725);
end;

SELECT * FROM CUSTOMER WHERE idc = 10;

explain plan for
SELECT * FROM CUSTOMER WHERE idc = 10;

select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'SELECT * FROM CUSTOMER WHERE idc = 10%';

begin
    PrintQueryStat('86jgh9p333zp9', 287150356);
end;

--6

select *
from Customer
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';
select * from table(dbms_xplan.display);

explain plan for
select *
from Customer
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select * from Customer where lname = %';

begin
    PrintQueryStat('byrrkp5uv9ypg', 169474562);
end;


select *
from CUSTOMER_CT
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';

explain plan for
select *
from CUSTOMER_CT
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select * from CUSTOMER_CT where lname = %';

begin
    PrintQueryStat('1mf5gzxugt03q', 4162219764);
end;

-- 7
create index customer_name_res on CUSTOMER_CT(lname, fname, residence);

select *
from CUSTOMER_CT
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';

explain plan for
select *
from CUSTOMER_CT
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select * from CUSTOMER_CT where lname = %';

begin
    PrintQueryStat('1mf5gzxugt03q', 905531940);
end;

--9

select idc
from Customer
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';
select * from table(dbms_xplan.display);

explain plan for
select idc
from Customer
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select idc from Customer where lname = %';

begin
    PrintQueryStat('byrrkp5uv9ypg', 169474562);
end;


select idc
from CUSTOMER_CT
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';

explain plan for
select idc
from CUSTOMER_CT
where lname = 'Müller' AND fname = 'Alena' AND residence = 'Praha';
select * from table(dbms_xplan.display);

SELECT * FROM vsql_user WHERE SQL_TEXT LIKE 'select idc from CUSTOMER_CT where lname = %';

begin
    PrintQueryStat('4vhz7ydxvag3w', 1665421821);
end;