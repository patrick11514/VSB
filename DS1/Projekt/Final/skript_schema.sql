-- DROPS
DROP TABLE IF EXISTS messages;
DROP TABLE IF EXISTS ticket_categories;
DROP TABLE IF EXISTS comments;
DROP TABLE IF EXISTS tickets;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS categories;


-- ======================= CATEGORIES =============================
CREATE TABLE categories (
    category_id  INTEGER IDENTITY PRIMARY KEY,
    name         VARCHAR(255) NOT NULL,
    description  VARCHAR(512) NOT NULL
);

-- ======================= USERS =============================
CREATE TABLE users (
    user_id   INTEGER IDENTITY PRIMARY KEY,
    username  VARCHAR(255) NOT NULL,
    email     VARCHAR(255),
    password  VARCHAR(72) NOT NULL,
    role      VARCHAR(1) NOT NULL
);

ALTER TABLE users ADD CONSTRAINT check_role CHECK ( role IN ( 'H', 'M' ) );

-- ======================= TICKETS =============================
CREATE TABLE tickets (
    ticket_id       INTEGER IDENTITY PRIMARY KEY,
    name            VARCHAR(255) NOT NULL,
    status          INTEGER NOT NULL,
    created_at      DATETIME NOT NULL,
    author          INTEGER NOT NULL,
    moderator       INTEGER NULL
);

ALTER TABLE tickets
    ADD CONSTRAINT tickets_users_fk FOREIGN KEY ( author )
        REFERENCES users ( user_id );

ALTER TABLE tickets
    ADD CONSTRAINT tickets_users_fkv2 FOREIGN KEY ( moderator )
        REFERENCES users ( user_id );

-- ======================= COMMENTS =============================
CREATE TABLE comments (
    comment_id         INTEGER IDENTITY PRIMARY KEY,
    comment            VARCHAR(1024) NOT NULL,
    commented_at       DATETIME NOT NULL,
    ticket_id          INTEGER NOT NULL,
    moderator          INTEGER NOT NULL
);

ALTER TABLE comments
    ADD CONSTRAINT comments_tickets_fk FOREIGN KEY ( ticket_id )
        REFERENCES tickets ( ticket_id );

ALTER TABLE comments
    ADD CONSTRAINT comments_users_fk FOREIGN KEY ( moderator )
        REFERENCES users ( user_id );


-- ======================= TICKET CATEGORIES =============================
CREATE TABLE ticket_categories (
    ticket_id               INTEGER NOT NULL,
    updated_at              DATETIME NOT NULL,
    moderator               INTEGER NULL,
    category_id             INTEGER NOT NULL
);

ALTER TABLE ticket_categories ADD CONSTRAINT ticket_categories_pk PRIMARY KEY ( ticket_id,
                                                                                updated_at );

ALTER TABLE ticket_categories
    ADD CONSTRAINT ticket_categories_cat_fk FOREIGN KEY ( category_id )
        REFERENCES categories ( category_id );

ALTER TABLE ticket_categories
    ADD CONSTRAINT ticket_categories_tickets_fk FOREIGN KEY ( ticket_id )
        REFERENCES tickets ( ticket_id );

ALTER TABLE ticket_categories
    ADD CONSTRAINT ticket_categories_users_fk FOREIGN KEY ( moderator )
        REFERENCES users ( user_id );


-- ======================= MESSAGES =============================
CREATE TABLE messages (
    message_id         INTEGER IDENTITY PRIMARY KEY,
    message            VARCHAR(1024) NOT NULL,
    created_at         DATETIME NOT NULL,
    ticket_id          INTEGER NOT NULL,
    author             INTEGER NOT NULL
);

ALTER TABLE messages
    ADD CONSTRAINT messages_tickets_fk FOREIGN KEY ( ticket_id )
        REFERENCES tickets ( ticket_id );

ALTER TABLE messages
    ADD CONSTRAINT messages_users_fk FOREIGN KEY ( author )
        REFERENCES users ( user_id );