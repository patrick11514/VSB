import type { AsyncReturnType, CreateContext } from '@patrick115/sveltekitapi';

export const context = (async () => {
    return {};
}) satisfies CreateContext;

export type Context = AsyncReturnType<typeof context>;
