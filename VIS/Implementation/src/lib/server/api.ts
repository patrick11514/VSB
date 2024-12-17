import { APICreate, MiddleWareError } from '@patrick115/sveltekitapi';
import type { Context } from './context';

export const api = new APICreate<Context>();

export const router = api.router;
export const procedure = api.procedure;
export const loginProcedure = procedure.use(async ({ ctx, next }) => {
    if (!ctx.logged) {
        throw new MiddleWareError({
            status: false,
            code: 403,
            message: 'Na toto nemáš přístup'
        });
    }

    return next(ctx.data);
});
