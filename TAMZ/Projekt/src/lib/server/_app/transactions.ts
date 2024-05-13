import type { BankRecord, Response, ResponseWithData } from '$/types/types';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import { z } from 'zod';
import { procedure } from '../api';
import { conn } from '../variables';

const list = procedure.POST.input(z.string()).query(async ({ input }) => {
    try {
        const transactions = await conn.selectFrom('records').selectAll().where('uuid', '=', input).orderBy('id desc').execute();

        return {
            status: true,
            data: transactions
        } satisfies ResponseWithData<BankRecord[]>;
    } catch (_) {
        return {
            status: false,
            code: 500,
            message: 'Internal Server Error'
        } satisfies ErrorApiResponse;
    }
});

const add = procedure.PUT.input(
    z.object({
        uuid: z.string(),
        value: z.number(),
        description: z.string()
    })
).query(async ({ input: { uuid, value, description } }) => {
    try {
        await conn.transaction().execute(async (trx) => {
            await trx
                .insertInto('records')
                .values({
                    uuid,
                    value,
                    description,
                    date: new Date()
                })
                .execute();

            let oldVal: number;

            const currentValue = await trx.selectFrom('balance').select('value').where('uuid', '=', uuid).executeTakeFirst();
            if (!currentValue) {
                await trx
                    .insertInto('balance')
                    .values({
                        uuid,
                        value: 0
                    })
                    .execute();

                oldVal = 0;
            } else {
                oldVal = currentValue.value;
            }

            await trx
                .updateTable('balance')
                .set({
                    uuid: uuid,
                    value: oldVal + value
                })
                .execute();
        });

        return {
            status: true
        } satisfies Response;
    } catch (_) {
        return {
            status: false,
            code: 500,
            message: 'Internal server error'
        } satisfies ErrorApiResponse;
    }
});

const remove = procedure.DELETE.input(
    z.object({
        id: z.number(),
        uuid: z.string()
    })
).query(async ({ input: { uuid, id } }) => {
    try {
        await conn.deleteFrom('records').where('uuid', '=', uuid).where('id', '=', id).execute();

        return {
            status: true
        } satisfies Response;
    } catch (_) {
        return {
            status: false,
            code: 500,
            message: 'Internal server error'
        } satisfies ErrorApiResponse;
    }
});

const update = procedure.PATCH.input(
    z.object({
        id: z.number(),
        uuid: z.string(),
        value: z.number(),
        description: z.string()
    })
).query(async ({ input: { uuid, value, description, id } }) => {
    try {
        await conn
            .updateTable('records')
            .set({
                value: value,
                description: description
            })
            .where('uuid', '=', uuid)
            .where('id', '=', id)
            .execute();

        return {
            status: true
        } satisfies Response;
    } catch (_) {
        return {
            status: false,
            code: 500,
            message: 'Internal server error'
        } satisfies ErrorApiResponse;
    }
});

export default [list, add, remove, update];
