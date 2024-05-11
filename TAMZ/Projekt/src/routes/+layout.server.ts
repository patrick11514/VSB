import { Server } from '$/lib/server/server';
import { jwt } from '$/lib/server/variables';
import type { UserData } from '$/types/types';
import type { LayoutServerLoad } from './$types';

export const load = (async ({ cookies }) => {
    const hydrationData = Server.hydrateToClient();

    const cookie = cookies.get('session');
    if (!cookie) {
        return {
            hydrationData,
            userData: {
                banks: []
            }
        };
    }

    const data = jwt.getCookie<UserData>(cookie);

    if (!data) {
        return {
            hydrationData,
            userData: {
                banks: []
            }
        };
    }

    return {
        hydrationData,
        userData: data
    };
}) satisfies LayoutServerLoad;
