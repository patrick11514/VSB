create or replace procedure PrintPages_unused_space(
    p_table_name varchar,  p_user_name varchar, p_type varchar)
as
    blocks           number;
    bytes            number;
    unused_blocks    number;
    unused_bytes     number;
    expired_blocks   number;
    expired_bytes    number;
    unexpired_blocks number;
    unexpired_bytes  number;
    mega number := 1024.0 * 1024.0;
begin
    dbms_space.unused_space(p_user_name, p_table_name, p_type,
                            blocks, bytes, unused_blocks, unused_bytes, expired_blocks,
                            expired_bytes, unexpired_blocks, unexpired_bytes);

    dbms_output.put_line('blocks: ' || blocks);
    dbms_output.put_line('size (MB): ' || (bytes / mega));
    dbms_output.put_line('used_blocks: ' ||
                         (blocks - unused_blocks));
    dbms_output.put_line('size used (MB): ' ||
                         ((bytes / mega) - (unused_bytes / mega)));
    dbms_output.put_line('unused_blocks: ' || unused_blocks);
    dbms_output.put_line('size unused (MB): ' ||
                         (unused_bytes / mega));
end;

-- Variant IV
create or replace procedure PrintPages_space_usage(
    p_table_name varchar, p_user_name varchar, p_type varchar)
as
    unformatted_blocks NUMBER;
    unformatted_bytes  NUMBER;
    fs1_blocks         NUMBER;
    fs1_bytes          NUMBER;
    fs2_blocks         NUMBER;
    fs2_bytes          NUMBER;
    fs3_blocks         NUMBER;
    fs3_bytes          NUMBER;
    fs4_blocks         NUMBER;
    fs4_bytes          NUMBER;
    full_blocks        NUMBER;
    full_bytes         NUMBER;
begin
    dbms_space.space_usage(p_user_name, p_table_name, p_type,
                           unformatted_blocks, unformatted_bytes, fs1_blocks, fs1_bytes,
                           fs2_blocks, fs2_bytes, fs3_blocks, fs3_bytes, fs4_blocks,
                           fs4_bytes, full_blocks, full_bytes, null);

    dbms_output.put_line('unformatted_blocks: ' ||
                         unformatted_blocks);
    dbms_output.put_line('fs1_blocks (0 to 25% free space): ' ||
                         fs1_blocks);
    dbms_output.put_line('fs2_blocks (25 to 50% free space): ' ||
                         fs2_blocks);
    dbms_output.put_line('fs3_blocks (50 to 75% free space): ' ||
                         fs3_blocks);
    dbms_output.put_line('fs4_blocks (75 to 100% free space): ' ||
                         fs4_blocks);
    dbms_output.put_line('full_blocks: ' || full_blocks);
end;

SET SERVEROUTPUT ON;

select table_name,blocks, empty_blocks,pct_free,pct_used from user_tables
where table_name='CUSTOMER';

select blocks, bytes/1024/1024 as MB from user_segments
where segment_name = 'CUSTOMER';

begin    
 PrintPages_unused_space('CUSTOMER', 'MIN0150', 'TABLE');
 PrintPages_space_usage('CUSTOMER', 'MIN0150', 'TABLE');
end;
/

select table_name,blocks, empty_blocks,pct_free,pct_used from user_tables
where table_name='CUSTOMER';


select blocks, bytes/1024/1024 as MB from user_segments
where segment_name = 'STORE';

begin
    PrintPages_unused_space('STORE', 'MIN0150', 'TABLE');
    PrintPages_space_usage('STORE', 'MIN0150', 'TABLE');
end;
/

select table_name,blocks, empty_blocks,pct_free,pct_used from user_tables
where table_name='STORE';


select blocks, bytes/1024/1024 as MB from user_segments
where segment_name = 'ORDERITEM';

begin
    PrintPages_unused_space('ORDERITEM', 'MIN0150', 'TABLE');
    PrintPages_space_usage('ORDERITEM', 'MIN0150', 'TABLE');
end;
/

select table_name,blocks, empty_blocks,pct_free,pct_used from user_tables
where table_name='ORDERITEM';

DELETE FROM ORDERITEM WHERE MOD(ido, 5)= 0;