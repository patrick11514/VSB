import type { UserData, UserState } from '$/types/types';
import type { AsyncReturnType, CreateContext } from '@patrick115/sveltekitapi';
import { jwt } from './variables';

export const context = (async ({ cookies }) => {
    const cookie = cookies.get('session');

    if (!cookie) {
        return {
            logged: false
        } satisfies UserState;
    }

    const data = jwt.getCookie<UserData>(cookie);
    if (!data) {
        return {
            logged: false
        } satisfies UserState;
    }

    return {
        logged: true,
        data
    } satisfies UserState;
}) satisfies CreateContext;

export type Context = AsyncReturnType<typeof context>;
