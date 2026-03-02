create or alter procedure PrintIndexes
@table VARCHAR(30)
as
select i.name as indexName
from sys.indexes i
         inner join sys.tables t on t.object_id = i.object_id
where T.Name = @table and i.name is not null;
go

exec PrintIndexes 'Store';
exec PrintIndexes 'Customer';
exec PrintIndexes 'OrderItem';

create or alter procedure PrintPagesIndex
@index varchar(30)
as
select
    i.name as IndexName,
    p.rows as ItemCounts,
    sum(a.total_pages) as TotalPages,
    round(cast(sum(a.total_pages) * 8 as float) / 1024, 1)
        as TotalPages_MB,
    sum(a.used_pages) as UsedPages,
    round(cast(sum(a.used_pages) * 8 as float) / 1024, 1)
        as UsedPages_MB
from sys.indexes i
         inner join sys.partitions p
                    on i.object_id = p.OBJECT_ID and i.index_id = p.index_id
         inner join sys.allocation_units a
                    on p.partition_id = a.container_id
where i.name = @index
group by i.name, p.Rows
order by i.name
go

exec PrintPagesHeap 'Store';
exec PrintPagesIndex 'PK__Store__9DBB2CF28B8A5A6D';

exec PrintPagesHeap 'Customer';
exec PrintPagesIndex 'PK__Customer__DC501A0CF8AE732B';

exec PrintPagesHeap 'OrderItem';
exec PrintPagesIndex 'pk_orderitem';


alter table dbo.OrderItem drop constraint pk_orderitem;
exec PrintIndexes 'OrderItem'; -- No index

alter table dbo.OrderItem add primary key (ido, idp); --3s
exec PrintIndexes 'OrderItem'; -- PK__OrderIte__C1951BA10FE7E718

create or alter procedure PrintIndexStats @user varchar(30), @table varchar(30), @index varchar(30)
as
select i.name, s.index_depth - 1 as height,
       sum(s.page_count) as page_count
from sys.dm_db_index_physical_stats(DB_ID(@user),
                                    OBJECT_ID(@table), NULL, NULL , 'DETAILED') s
         join sys.indexes i
              on s.object_id=i.object_id and s.index_id=i.index_id
where name=@index
group by i.name, s.index_depth
go

create or alter procedure PrintIndexLevelStats @user varchar(30), @table varchar(30), @index varchar(30)
as
select s.index_level as level, s.page_count,
       s.record_count, s.avg_record_size_in_bytes
                     as avg_record_size,
       round(s.avg_page_space_used_in_percent,1)
                     as page_utilization,
       round(s.avg_fragmentation_in_percent,2) as avg_frag
from sys.dm_db_index_physical_stats(DB_ID(@user),
                                    OBJECT_ID(@table), NULL, NULL , 'DETAILED') s
         join sys.indexes i
              on s.object_id=i.object_id and s.index_id=i.index_id
where name=@index
go

exec PrintIndexStats 'min0150', 'OrderItem', 'PK__OrderIte__C1951BA10FE7E718'
exec PrintIndexLevelStats 'min0150', 'OrderItem', 'PK__OrderIte__C1951BA10FE7E718'

-- nejmensi
exec PrintIndexLevelStats 'min0150', 'Customer', 'PK__Customer__DC501A0CF8AE732B'
exec PrintIndexStats'min0150', 'Store', 'PK__Store__9DBB2CF21C4BB04A'
exec PrintIndexLevelStats 'min0150', 'Store', 'PK__Store__9DBB2CF21C4BB04A' -- 74.1 % listový a 0.5% vnitřní
exec PrintIndexLevelStats 'min0150', 'OrderItem', 'PK__OrderIte__C1951BA10FE7E718'

ALTER INDEX PK__Store__9DBB2CF21C4BB04A ON Store REORGANIZE;

exec PrintIndexStats'min0150', 'Store', 'PK__Store__9DBB2CF21C4BB04A'
exec PrintIndexLevelStats 'min0150', 'Store', 'PK__Store__9DBB2CF21C4BB04A'


exec PrintIndexLevelStats 'min0150', 'Customer', 'PK__Customer__DC501A0CB1CBE065' -- 99.7 / 53.7 / 0.3 
ALTER INDEX PK__Customer__DC501A0CB1CBE065 ON Customer REORGANIZE;
exec PrintIndexLevelStats 'min0150', 'Customer', 'PK__Customer__DC501A0CB1CBE065'
