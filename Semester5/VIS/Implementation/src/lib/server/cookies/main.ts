import jwt from 'jsonwebtoken';
import path, { dirname } from 'path';
import JSONdb from 'simple-json-db';
import { fileURLToPath } from 'url';
import { v4 as uuid } from 'uuid';

/**
 * @author patrick115 Patrik Mintěl
 * @license MIT
 * @version 1.0.0
 * @description Cookie manager
 */

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

interface cookie<Type> {
    expires: number;
    values: Type;
}

export class SessionCookies {
    private db: JSONdb<cookie<unknown>>;

    constructor(storage = './cookies.json') {
        this.db = new JSONdb<cookie<unknown>>(storage.startsWith('./') || storage.startsWith('../') ? path.join(__dirname, storage) : storage, {
            syncOnWrite: true,
            jsonSpaces: false,
            asyncWrite: false
        });
    }

    checkCookies() {
        const cookies = Object.entries(this.db.JSON());
        cookies.forEach(([key, value]) => {
            if (value.expires < Date.now()) {
                this.db.delete(key);
            }
        });
    }

    getCookie<T>(key: string) {
        this.checkCookies();
        return this.db.get(key) as cookie<T>;
    }

    updateCookie<T>(id: string, value: T, age: number) {
        this.checkCookies();

        this.db.set(id, {
            expires: Date.now() + age,
            values: value
        });

        return id;
    }

    setCookie<T>(value: T, age: number) {
        this.checkCookies();

        let id = uuid();

        while (this.db.has(id)) {
            id = uuid();
        }

        this.db.set(id, {
            expires: Date.now() + age,
            values: value
        });

        return id;
    }

    deleteCookie(key: string) {
        this.checkCookies();
        this.db.delete(key);
    }
}

export class JWTCookies {
    private key: string;
    constructor(key: string) {
        this.key = key;
    }

    setCookie(value: object | string | Buffer) {
        return jwt.sign(value, this.key);
    }

    getCookie<T>(token: string): T | null {
        try {
            return jwt.verify(token, this.key) as T;
        } catch (_) {
            return null;
        }
    }
}
