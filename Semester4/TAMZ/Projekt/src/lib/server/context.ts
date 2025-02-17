import type { UserData } from '$/types/types';
import type { AsyncReturnType, CreateContext } from '@patrick115/sveltekitapi';
import { jwt } from './variables';

export const createContext = (async ({ cookies }) => {
    const cookie = cookies.get('session');

    if (!cookie) {
        return {
            banks: []
        };
    }

    const data = jwt.getCookie<UserData>(cookie);

    if (!data) {
        return {
            banks: []
        };
    }

    return data;
}) satisfies CreateContext;

export type Context = AsyncReturnType<typeof createContext>;
