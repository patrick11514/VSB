import type { BankTarget, Response, ResponseWithData } from '$/types/types';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import { z } from 'zod';
import { procedure } from '../api';
import { conn } from '../variables';

const list = procedure.POST.input(z.string()).query(async ({ input }) => {
    try {
        const targets = await conn.selectFrom('targets').selectAll().where('uuid', '=', input).orderBy('id desc').execute();

        return {
            status: true,
            data: targets
        } satisfies ResponseWithData<BankTarget[]>;
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
        name: z.string(),
        description: z.string(),
        value: z.number()
    })
).query(async ({ input: { uuid, name, description, value } }) => {
    try {
        await conn
            .insertInto('targets')
            .values({
                uuid,
                name,
                description,
                targetValue: value,
                date: new Date()
            })
            .execute();

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

const remove = procedure.DELETE.input(
    z.object({
        id: z.number(),
        uuid: z.string()
    })
).query(async ({ input: { uuid, id } }) => {
    try {
        await conn.deleteFrom('targets').where('uuid', '=', uuid).where('id', '=', id).execute();

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

const edit = procedure.PATCH.input(
    z.object({
        id: z.number(),
        uuid: z.string(),
        name: z.string(),
        description: z.string(),
        value: z.number()
    })
).query(async ({ input: { uuid, id, description, name, value } }) => {
    try {
        await conn
            .updateTable('targets')
            .set({
                name,
                description,
                targetValue: value
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
            message: 'Internal Server Error'
        } satisfies ErrorApiResponse;
    }
});

export default [list, add, remove, edit];
