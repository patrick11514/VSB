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
            .leftJoin('records as rev', (join) =>
                join
                    .on(sql`rev.uuid = bank.uuid`)
                    .on(sql`MONTH(CURDATE()) = MONTH(rev.date)`)
                    .on('rev.value', '>', 0)
            )
            .leftJoin('records as exp', (join) =>
                join
                    .on(sql`exp.uuid = bank.uuid`)
                    .on(sql`MONTH(CURDATE()) = MONTH(exp.date)`)
                    .on('exp.value', '<', 0)
            )
            .select([
                'bank.uuid',
                'bank.name',
                'bank.password',
                'balance.value',
                sql<number>`COALESCE(SUM(rev.value), 0)`.as('revenues'),
                sql<number>`COALESCE(SUM(exp.value), 0)`.as('expenses')
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
