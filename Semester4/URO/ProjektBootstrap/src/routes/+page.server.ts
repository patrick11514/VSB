import { Server } from '$/lib/server/server';
import type { PageServerLoad } from './$types';

export const load = (async (ev) => {
    return {
        data: await Server.ssr.data.GET(ev)
    };
}) satisfies PageServerLoad;
