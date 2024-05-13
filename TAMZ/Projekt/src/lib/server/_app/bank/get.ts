import type { BankWithoutHash, ResponseWithData } from '$/types/types';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import { sql } from 'kysely';
import { z } from 'zod';
import { procedure } from '../../api';
import { conn } from '../../variables';

const endpoint = procedure.POST.input(z.string()).query(async ({ input, ctx }) => {
    if (!ctx.banks.find((bank) => bank.uuid === input)) {
        return {
            status: false,
            code: 403,
            message: 'No permission'
        } satisfies ErrorApiResponse;
    }

    /*
[
            'bank.uuid',
            'bank.name',
            'bank.password',
            'balance.value',
            sql<number>`COALESCE(SUM(rev.value), 0)`.as('revenues'),
            sql<number>`COALESCE(SUM(exp.value), 0)`.as('expenses')
        ]) */

    const bank = await conn
        .selectFrom('bank')
        .leftJoin('balance', 'balance.uuid', 'bank.uuid')
        .select(({ selectFrom }) => [
            'bank.uuid',
            'bank.name',
            'balance.value',
            selectFrom('records')
                .select([sql<number>`COALESCE(SUM(records.value), 0)`.as('revenues')])
                .whereRef('records.uuid', '=', 'balance.uuid')
                .where('records.value', '>', 0)
                .where(sql<boolean>`MONTH(CURDATE()) = MONTH(records.date)`)
                .as('revenues'),
            selectFrom('records')
                .select([sql<number>`COALESCE(SUM(records.value), 0)`.as('expenses')])
                .whereRef('records.uuid', '=', 'balance.uuid')
                .where('records.value', '<', 0)
                .where(sql<boolean>`MONTH(CURDATE()) = MONTH(records.date)`)
                .as('expenses')
        ])
        .where('bank.uuid', '=', input)
        .executeTakeFirst();

    if (!bank) {
        return {
            status: false,
            code: 404,
            message: 'Not found'
        } satisfies ErrorApiResponse;
    }

    const newBank = {
        ...bank,
        balance: bank.value ?? 0,
        revenues: bank.revenues ?? 0,
        expenses: bank.expenses ?? 0,
        password: undefined,
        value: undefined
    };

    return {
        status: true,
        data: newBank
    } satisfies ResponseWithData<BankWithoutHash>;
});

export default endpoint;
