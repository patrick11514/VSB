create or alter procedure PrintPages @tableName varchar(30), @indexId int
AS
SELECT
    t.NAME AS TableName,
    p.rows AS RowCounts,
    SUM(a.total_pages) AS TotalPages,
    round(cast(SUM(a.total_pages) * 8 as float) / 1024, 1) AS TotalPages_MB,
    SUM(a.used_pages) AS UsedPages,
    round(cast(SUM(a.used_pages) * 8 as float) / 1024, 1) AS UsedPages_MB
FROM sys.tables t
         INNER JOIN
     sys.indexes i ON t.OBJECT_ID = i.object_id
         INNER JOIN
     sys.partitions p ON i.object_id = p.OBJECT_ID AND
                         i.index_id = p.index_id
         INNER JOIN
     sys.allocation_units a ON p.partition_id = a.container_id
WHERE t.NAME = @tableName and p.index_id = @indexId
GROUP BY t.Name, p.Rows
ORDER BY t.Name
go

create or alter procedure PrintPagesHeap @tableName varchar(30)
as
    exec PrintPages @tableName, 0
go

exec PrintPagesHeap 'Customer';
exec PrintPagesHeap 'Store';
exec PrintPagesHeap 'OrderItem';

DELETE FROM OrderItem WHERE ido % 5 = 0;

alter table Customer rebuild;
alter table Store rebuild;
alter table OrderItem rebuild;