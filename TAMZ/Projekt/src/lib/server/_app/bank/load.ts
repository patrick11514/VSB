import { procedure } from '$/lib/server/api';
import { conn } from '$/lib/server/variables';
import type { Bank } from '$/types/database';
import type { BankWithoutHash, ResponseWithData } from '$/types/types';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import bcrypt from 'bcrypt';
import { sql } from 'kysely';

const endpoint = procedure.GET.query(async ({ ctx }) => {
    if (ctx.banks.length === 0) {
        return {
            status: true,
            data: []
        } satisfies ResponseWithData<Bank[]>;
    }

    try {
        const banks = await conn
            .selectFrom('bank')
            .leftJoin('balance', 'balance.uuid', 'bank.uuid')
            .select(({ selectFrom }) => [
                'bank.uuid',
                'bank.name',
                'balance.value',
                'bank.password',
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
            .where(
                'bank.uuid',
                'in',
                ctx.banks.map((bank) => bank.uuid)
            )
            .execute();

        const newBanks: BankWithoutHash[] = [];

        for (const bank of banks) {
            const pass = ctx.banks.find((b) => b.uuid === bank.uuid)?.password;

            if (!pass) {
                continue;
            }

            if (bcrypt.compareSync(pass, bank.password)) {
                let newBank = {
                    ...bank,
                    balance: bank.value ?? 0,
                    revenues: bank.revenues ?? 0,
                    expenses: bank.expenses ?? 0,
                    password: undefined,
                    value: undefined
                };

                newBanks.push(newBank);
            }
        }

        return {
            status: true,
            data: newBanks
        } satisfies ResponseWithData<BankWithoutHash[]>;
    } catch (_) {
        return {
            status: false,
            code: 500,
            message: 'Internal Server Error'
        } satisfies ErrorApiResponse;
    }
});

export default endpoint;
