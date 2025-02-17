import { OnlineBankSchema } from '$/lib';
import type { Response } from '$/types/types';
import { HASH_ROUNDS } from '$env/static/private';
import { type ErrorApiResponse } from '@patrick115/sveltekitapi';
import bcrypt from 'bcrypt';
import { z } from 'zod';
import { procedure } from '../api';
import { conn, jwt } from '../variables';

const endpoint = procedure.POST.input(z.any()).query(async ({ input, ctx, ev }) => {
    try {
        const parsed = OnlineBankSchema.parse(input);

        const exists = await conn.selectFrom('bank').select('uuid').where('uuid', '=', parsed.uuid).executeTakeFirst();
        if (exists) {
            return {
                status: false,
                code: 402,
                message: '0'
            } satisfies ErrorApiResponse;
        }

        await conn.transaction().execute(async (trx) => {
            await trx
                .insertInto('bank')
                .values({
                    uuid: parsed.uuid,
                    name: parsed.name,
                    password: bcrypt.hashSync(parsed.password, parseInt(HASH_ROUNDS))
                })
                .execute();

            await trx
                .insertInto('balance')
                .values({
                    uuid: parsed.uuid,
                    value: parsed.balance
                })
                .execute();

            if (parsed.records.length > 0) {
                await trx
                    .insertInto('records')
                    .values(
                        parsed.records.map((record) => {
                            return {
                                uuid: record.uuid,
                                value: record.value,
                                description: record.description,
                                date: new Date(record.date)
                            };
                        })
                    )
                    .execute();
            }

            if (parsed.targets.length > 0) {
                await trx
                    .insertInto('targets')
                    .values(
                        parsed.targets.map((target) => {
                            return {
                                uuid: target.uuid,
                                description: target.description,
                                name: target.name,
                                targetValue: target.targetValue,
                                date: new Date(target.date)
                            };
                        })
                    )
                    .execute();
            }
        });

        const current = ctx;
        current.banks.push({
            uuid: parsed.uuid,
            password: parsed.password
        });

        const cookie = jwt.setCookie(current);

        ev.cookies.set('session', cookie, {
            path: '/'
        });

        return {
            status: true
        } satisfies Response;
    } catch (_) {
        console.log(_);
        return {
            status: false,
            code: 402,
            message: '-1'
        } satisfies ErrorApiResponse;
    }
});

export default endpoint;
