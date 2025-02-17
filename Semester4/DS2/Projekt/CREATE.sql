

create table USERS
(
    USER_ID  NUMBER generated as identity
        constraint USERS_PK
            primary key,
    USERNAME VARCHAR2(255) not null,
    EMAIL    VARCHAR2(255),
    PASSWORD VARCHAR2(72)  not null
);

create table GROUPS
(
    GROUP_ID NUMBER generated as identity
        constraint GROUPS_PK
            primary key,
    NAME     VARCHAR2(255) not null
);

create table SERVERS
(
    SERVER_ID NUMBER generated as identity
        constraint SERVERS_PK
            primary key,
    NAME      VARCHAR2(255) not null
);

create table PERMISSIONS
(
    PERMISSIONS_ID NUMBER generated as identity
        constraint PERMISSIONS_PK
            primary key,
    NAME           VARCHAR2(255) not null,
    REMOVE         NUMBER        not null,
    USER_ID        NUMBER        not null
        constraint PERMISSIONS_USERS_USER_ID_FK
            references USERS,
    SERVER_ID      NUMBER        not null
        constraint PERMISSIONS_SERVERS_SERVER_ID_FK
            references SERVERS
);

create table USER_HAVE_GROUP
(
    GROUP_ID  NUMBER not null
        constraint USER_HAVE_GROUP_GROUPS_GROUP_ID_FK
            references GROUPS,
    USER_ID   NUMBER not null
        constraint USER_HAVE_GROUP_USERS_USER_ID_FK
            references USERS,
    SERVER_ID NUMBER not null
        constraint USER_HAVE_GROUP_SERVERS_SERVER_ID_FK
            references SERVERS,
    constraint USER_HAVE_GROUP_PK
        primary key (SERVER_ID, USER_ID, GROUP_ID)
);