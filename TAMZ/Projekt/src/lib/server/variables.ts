import type { DB } from '$/types/database';
import { DATABASE_DATABASE, DATABASE_HOST, DATABASE_NAME, DATABASE_PASSWORD, DATABASE_PORT, JWT_SECRET } from '$env/static/private';
import { Kysely, MysqlDialect } from 'kysely';
import { createPool } from 'mysql2';
import { JWTCookies } from './cookies/main';

export const jwt = new JWTCookies(JWT_SECRET);

const dialect = new MysqlDialect({
    pool: createPool({
        host: DATABASE_HOST,
        port: parseInt(DATABASE_PORT),
        user: DATABASE_NAME,
        password: DATABASE_PASSWORD,
        database: DATABASE_DATABASE
    })
});

export const conn = new Kysely<DB>({
    dialect,
    log: ['query']
});
