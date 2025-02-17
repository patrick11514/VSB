import { Server } from '$/lib/server/server';
import { jwt } from '$/lib/server/variables';
import type { UserData, UserState } from '$/types/types';
import type { LayoutServerLoad } from './$types';

export const load = (async ({ cookies }) => {
    let status: UserState = {
        logged: false
    };

    const cookie = cookies.get('session');

    if (cookie) {
        const data = jwt.getCookie<UserData>(cookie);
        if (data) {
            status = {
                logged: true,
                data
            };
        }
    }

    return {
        api: Server.hydrateToClient(),
        user: status
    };
}) satisfies LayoutServerLoad;
