    -- ProductOrderDb, A database for Physical Database Design
    -- SQL Server Version
    --
    -- Michal Kratky, Radim Baca
    -- dbedu@cs.vsb.cz, 2023-2026
    -- last update: 2026-02-10
    
    -- Drop section
    drop table IF EXISTS OrderItem
    drop table IF EXISTS "Order"
    drop table IF EXISTS Staff
    drop table IF EXISTS Store
    drop table IF EXISTS Product
    drop table IF EXISTS Customer
    
    -- Create section
    create table Customer (
                              idc int primary key nonclustered,
                              fname varchar(20) not null,
                              lname varchar(30) not null,
                              residence varchar(20) not null,
                              gender char(1) not null,
                              birthday date not null
    );
    
    create table Product (
                             idp int primary key nonclustered,
                             name varchar(30) not null,
                             unit_price int not null,
                             producer varchar(30) not null,
                             description varchar(2000) null
    );
    
    create table Store (
                           idso int primary key nonclustered,
                           name varchar(30) not null,
                           residence varchar(20) not null
    );
    
    create table Staff (
                           idsa varchar(7) primary key nonclustered,
                           fname varchar(20) not null,
                           lname varchar(30) not null,
                           residence varchar(20) not null,
                           gender char(1) not null,
                           birthday date not null,
                           start_contract date not null,
                           end_contract date default null,
                           idso int references Store(idso) not null
    );
    
    create table "Order" (
                             ido int primary key nonclustered,
                             order_datetime date not null,
                             idc int references Customer(idc) not null,
                             order_status varchar(10),
                             idso int references Store(idso) not null,
                             idsa varchar(7) references Staff(idsa) not null
    );
    
    create table OrderItem (
                               ido int references "Order"(ido) not null,
                               idp int references Product(idp) not null,
                               unit_price bigint not null,
                               quantity int not null
    );
    
    alter table OrderItem add constraint pk_orderitem primary key nonclustered(ido, idp);
    
    -- Copy records section
    insert into Customer select * from ProductOrder.dbo.Customer;
    insert into Product select * from ProductOrder.dbo.Product;
    insert into Store select * from ProductOrder.dbo.Store;
    insert into Staff select * from ProductOrder.dbo.Staff;
    insert into "Order" select * from ProductOrder.dbo."Order";
    insert into OrderItem select * from ProductOrder.dbo.OrderItem;
    
    -- Print the row count
    select 'Customer' as table_name, count(*) as row_count from Customer
    union
    select 'Product' as table_name, count(*) as row_count from Product
    union
    select 'Store' as table_name, count(*) as row_count from Store
    union
    select 'Staff' as table_name, count(*) as row_count from Staff
    union
    select 'Order' as table_name, count(*) as row_count from "Order"
    union
    select 'OrderItem' as table_name, count(*) as row_count from OrderItem;