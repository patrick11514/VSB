import type { DB } from '$/types/database';
import { JWT_SECRET, DATABASE_NAME, DATABASE_IP, DATABASE_PASSWORD, DATABASE_PORT, DATABASE_USER } from '$env/static/private';
import { JWTCookies } from './cookies/main';
import { Kysely, MysqlDialect } from 'kysely';
import { createPool } from 'mysql2';

export const jwt = new JWTCookies(JWT_SECRET);
const dialect = new MysqlDialect({
    pool: createPool({
        host: DATABASE_IP,
        port: parseInt(DATABASE_PORT),
        user: DATABASE_USER,
        password: DATABASE_PASSWORD,
        database: DATABASE_NAME
    })
});

export const conn = new Kysely<DB>({
    dialect
});
