import type { OnlineBankSchema } from '$/lib';
import type { ResponseWithData } from '$/types/types';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import { z } from 'zod';
import { procedure } from '../api';
import { conn } from '../variables';

const endpoint = procedure.POST.input(z.string()).query(async ({ input, ctx }) => {
    const bank = await conn.selectFrom('bank').selectAll().where('uuid', '=', input).executeTakeFirst();
    if (!bank) {
        return {
            status: false,
            code: 404,
            message: 'Not found'
        } satisfies ErrorApiResponse;
    }

    //get password
    const bankLocalData = ctx.banks.find((bank) => bank.uuid === input);

    if (!bankLocalData) {
        return {
            status: false,
            code: 403,
            message: 'Forbidden'
        } satisfies ErrorApiResponse;
    }

    const balance = await conn.selectFrom('balance').selectAll().where('uuid', '=', input).executeTakeFirst();
    if (!balance) {
        return {
            status: false,
            code: 404,
            message: 'Not found'
        } satisfies ErrorApiResponse;
    }

    try {
        const records = await conn.selectFrom('records').selectAll().where('uuid', '=', input).execute();

        const targets = await conn.selectFrom('targets').selectAll().where('uuid', '=', input).execute();

        return {
            status: true,
            data: {
                local: false,
                ...bank,
                password: bankLocalData.password,
                balance: balance.value,
                records: records.map((r) => {
                    return {
                        ...r,
                        date: r.date.getTime()
                    };
                }),
                targets: targets.map((t) => {
                    return {
                        ...t,
                        date: t.date.getTime()
                    };
                })
            }
        } satisfies ResponseWithData<z.infer<typeof OnlineBankSchema>>;
    } catch (_) {
        return {
            status: false,
            code: 500,
            message: 'Internal server error'
        } satisfies ErrorApiResponse;
    }
});

export default endpoint;
