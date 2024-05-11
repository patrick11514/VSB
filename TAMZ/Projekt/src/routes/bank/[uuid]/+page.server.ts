import { conn, jwt } from '$/lib/server/variables';
import type { UserData } from '$/types/types';
import { redirect } from '@sveltejs/kit';
import bcrypt from 'bcrypt';
import type { PageServerLoad } from './$types';

export const load = (async ({ params, cookies }) => {
    const bank = await conn.selectFrom('bank').select('password').where('uuid', '=', params.uuid).executeTakeFirst();
    if (!bank) {
        redirect(302, '/');
    }

    const cookie = cookies.get('session');
    if (!cookie) {
        redirect(302, '/');
    }

    const data = jwt.getCookie<UserData>(cookie);

    if (!data) {
        redirect(302, '/');
    }

    const bankData = data.banks.find((b) => b.uuid === params.uuid);

    if (!bankData) {
        redirect(302, '/');
    }

    if (!bcrypt.compareSync(bankData.password, bank.password)) {
        redirect(302, '/');
    }
}) satisfies PageServerLoad;
