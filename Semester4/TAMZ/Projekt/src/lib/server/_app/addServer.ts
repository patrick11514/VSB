import type { Response } from '$/types/types';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import bcrypt from 'bcrypt';
import { z } from 'zod';
import { procedure } from '../api';
import { conn, jwt } from '../variables';

const endpoint = procedure.POST.input(
    z.object({
        uuid: z.string(),
        password: z.string()
    })
).query(async ({ input, ctx, ev }) => {
    const bankIndex = ctx.banks.findIndex((bank) => bank.uuid === input.uuid);

    if (bankIndex !== -1 && ctx.banks[bankIndex].password === input.password) {
        return {
            status: false,
            code: 402,
            message: 'Účet byl již přidán'
        } satisfies ErrorApiResponse;
    }

    const result = await conn.selectFrom('bank').select('password').where('uuid', '=', input.uuid).executeTakeFirst();
    if (!result) {
        return {
            status: false,
            code: 404,
            message: 'Účet nenalezen'
        } satisfies ErrorApiResponse;
    }

    if (!bcrypt.compareSync(input.password, result.password)) {
        return {
            status: false,
            code: 401,
            message: 'Špatné heslo'
        } satisfies ErrorApiResponse;
    }

    const current = ctx;

    if (bankIndex === -1) {
        current.banks.push({
            uuid: input.uuid,
            password: input.password
        });
    } else {
        current.banks[bankIndex].password = input.password;
    }

    const cookie = jwt.setCookie(current);

    ev.cookies.set('session', cookie, {
        path: '/'
    });

    return {
        status: true
    } satisfies Response;
});

export default endpoint;
