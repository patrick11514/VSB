import type { Response } from '$/types/types';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import { z } from 'zod';
import { procedure } from '../../api';
import { conn } from '../../variables';
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
        name: z.string()
    })
).query(async ({ input: { name, uuid } }) => {
    try {
        await conn
            .updateTable('bank')
            .set({
                name
            })
            .where('uuid', '=', uuid)
            .execute();

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
