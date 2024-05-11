import type { BankRecord, ResponseWithData } from '$/types/types';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import { z } from 'zod';
import { procedure } from '../api';
import { conn } from '../variables';

const endpoint = procedure.POST.input(z.string()).query(async ({ input }) => {
    try {
        const transactions = await conn.selectFrom('records').selectAll().where('uuid', '=', input).execute();

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

export default endpoint;
