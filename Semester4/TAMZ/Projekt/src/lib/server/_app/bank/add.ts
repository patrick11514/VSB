import { procedure } from '$/lib/server/api';
import type { Response } from '$/types/types';
import { HASH_ROUNDS } from '$env/static/private';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import bcrypt from 'bcrypt';
import * as uuid from 'uuid';
import { z } from 'zod';
import { conn, jwt } from '../../variables';

const endpoint = procedure.PUT.input(
    z.object({
        name: z.string(),
        password: z.string()
    })
).query(async ({ ctx, input, ev }) => {
    try {
        const bankUuid = uuid.v4();
        await conn
            .insertInto('bank')
            .values({
                uuid: bankUuid,
                name: input.name,
                password: bcrypt.hashSync(input.password, parseInt(HASH_ROUNDS))
            })
            .execute();

        const cookie = jwt.setCookie({
            ...ctx,
            banks: [
                ...ctx.banks,
                {
                    uuid: bankUuid,
                    password: input.password
                }
            ]
        });

        ev.cookies.set('session', cookie, {
            path: '/'
        });

        return {
            status: true
        } satisfies Response;
    } catch (_) {
        return {
            status: false,
            code: 500,
            message: 'Internal Server Error'
        } satisfies ErrorApiResponse;
    }
});

export default endpoint;
