import type { Response } from '$/types/types';
import { HASH_ROUNDS } from '$env/static/private';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import bcrypt from 'bcrypt';
import { z } from 'zod';
import { procedure } from '../../api';
import { conn, jwt } from '../../variables';
import add from './add';
import get from './get';
import load from './load';

const remove = procedure.DELETE.input(z.string()).query(async ({ input }) => {
    try {
        await conn.deleteFrom('bank').where('uuid', '=', input).execute();

        return {
            status: true
        } satisfies Response;
    } catch (_) {
        return {
            status: false,
            code: 500,
            message: 'Internal server errror'
        } satisfies ErrorApiResponse;
    }
});

const update = procedure.PATCH.input(
    z.object({
        uuid: z.string(),
        name: z.string(),
        newPassword: z.string()
    })
).query(async ({ input: { name, uuid, newPassword }, ctx, ev }) => {
    try {
        await conn
            .updateTable('bank')
            .set({
                name,
                password: newPassword === '' ? undefined : bcrypt.hashSync(newPassword, parseInt(HASH_ROUNDS))
            })
            .where('uuid', '=', uuid)
            .execute();

        if (newPassword !== '') {
            const current = ctx;
            const index = ctx.banks.findIndex((bank) => bank.uuid === uuid);

            if (index === -1) {
                return {
                    status: true
                } satisfies Response;
            }

            current.banks[index].password = newPassword;

            const cookie = jwt.setCookie(current);

            ev.cookies.set('session', cookie, {
                path: '/'
            });
        }

        return {
            status: true
        } satisfies Response;
    } catch (_) {
        return {
            status: false,
            code: 500,
            message: 'Internal server errror'
        } satisfies ErrorApiResponse;
    }
});

export default [load, add, get, remove, update];
