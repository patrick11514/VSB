import { redirect } from '@sveltejs/kit';
import type { PageServerLoad } from './$types';
import { jwt } from '$/lib/server/variables';

export const load = (({ cookies }) => {
    const cookie = cookies.get('session');
    if (!cookie) {
        redirect(302, '/login');
    }

    const data = jwt.getCookie(cookie);

    if (!data) {
        redirect(302, '/login');
    }

    //Handling pro kontributora
}) satisfies PageServerLoad;
